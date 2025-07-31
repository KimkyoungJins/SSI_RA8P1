/* da7212_speaker_init.h */
#ifndef DA7212_SPEAKER_INIT_H_
#define DA7212_SPEAKER_INIT_H_

#include "bsp_api.h"    // fsp_err_t 정의

/**
 * @brief   DA7212 사운드 출력용 초기화 함수
 * @retval  FSP_SUCCESS            성공
 *          기타 FSP_ERR 코드      실패 원인
 */
fsp_err_t da7212_speaker_init(void);

#endif // DA7212_SPEAKER_INIT_H_


/* da7212_speaker_init.c */
#include "da7212_speaker_init.h"
#include "r_iic_master.h"   // I2C 드라이버 API
#include "bsp_api.h"       // BSP_DELAY_UNITS 정의
#include "common_utils.h"  // APP_ERR_PRINT, APP_ERR_TRAP 정의
#include "hal_data.h"


// 외부에서 정의된 I2C 마스터 컨트롤러와 설정
extern iic_master_instance_ctrl_t g_i2c_master0_ctrl;
extern const i2c_master_cfg_t     g_i2c_master0_cfg;
static fsp_err_t da7212_read_register(uint8_t reg_addr, uint8_t* p_data);
fsp_err_t da7212_set_speaker_volume_max(void);
fsp_err_t da7212_set_dac_volume_max(void);



/**
 * DA7212 레지스터 초기
 * @return
 */


fsp_err_t da7212_speaker_init(void)
{
    fsp_err_t err;
    uint8_t   reg[2];
    uint8_t   status;

    // GPIO 설정 (I2C 풀업용)
    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_05_PIN_12, BSP_IO_LEVEL_HIGH);
    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_05_PIN_11, BSP_IO_LEVEL_HIGH);
    R_BSP_SoftwareDelay(50, BSP_DELAY_UNITS_MILLISECONDS);

    // I2C 마스터 초기화
    err = R_IIC_MASTER_Open(&g_i2c_master0_ctrl, &g_i2c_master0_cfg);
    if (FSP_SUCCESS != err) {
        APP_ERR_PRINT("I2C open failed (0x%02X)\r\n", err);
        return err;
    }

    // 콜백 설정
    R_IIC_MASTER_CallbackSet(&g_i2c_master0_ctrl, i2c_master0_callback, NULL, NULL);

    // 통신 테스트 (STATUS1 레지스터 읽기)
    err = da7212_read_register(0x02, &status);
    if (FSP_SUCCESS != err) {
        APP_ERR_PRINT("DA7212 communication test failed (0x%02X)\r\n", err);
        return err;
    }

    // 1. SYSTEM_ACTIVE 설정
    reg[0] = 0xFD; reg[1] = 0x01;
    err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, reg, 2, false);
    if (FSP_SUCCESS != err) return err;
    R_BSP_SoftwareDelay(40, BSP_DELAY_UNITS_MILLISECONDS);

    // 2. BIAS_EN 활성화
    reg[0] = 0x23; reg[1] = 0x08;
    err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, reg, 2, false);
    if (FSP_SUCCESS != err) return err;
    R_BSP_SoftwareDelay(25, BSP_DELAY_UNITS_MILLISECONDS);

    // 3. 샘플레이트 48kHz 설정
    reg[0] = 0x22; reg[1] = 0x0B;
    err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, reg, 2, false);
    if (FSP_SUCCESS != err) return err;

    // 4. PLL 설정
    const struct { uint8_t addr, val; } pll_cfg[] = {
        {0x27, 0x84}, {0x24, 0x00}, {0x25, 0x00}, {0x26, 0x20}
    };

    for (size_t i = 0; i < sizeof(pll_cfg)/sizeof(pll_cfg[0]); ++i) {
        reg[0] = pll_cfg[i].addr;
        reg[1] = pll_cfg[i].val;
        err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, reg, 2, false);
        if (FSP_SUCCESS != err) return err;
    }

    R_BSP_SoftwareDelay(5, BSP_DELAY_UNITS_MILLISECONDS);

    // 5. PLL 상태 확인 (올바른 레지스터 사용)
    err = da7212_read_register(0x03, &status);  // PLL_STATUS 레지스터
    if ((FSP_SUCCESS != err) || !(status & 0x01)) {  // Bit 0: PLL_LOCK
        APP_ERR_PRINT("PLL lock fail (status=0x%02X)\r\n", status);
        return (err ? err : FSP_ERR_ABORTED);
    }

    // 6) DAI Clock Mode 설정
    reg[0] = 0x28; reg[1] = 0x81;
    err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, reg, 2, false);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("DA7212 init error: DAI clock mode failed (0x%02X)\r\n", err);
        return err;
    }

    // 7) DAI Control 설정
    reg[0] = 0x29; reg[1] = 0xC8;
    err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, reg, 2, false);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("DA7212 init error: DAI control failed (0x%02X)\r\n", err);
        return err;
    }

    // 8) DAC Routing 설정
    reg[0] = 0x2A; reg[1] = 0x32;
    err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, reg, 2, false);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("DA7212 init error: DAC routing failed (0x%02X)\r\n", err);
        return err;
    }

    // 9) Charge Pump 설정
    reg[0] = 0x47; reg[1] = 0xA1;
    err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, reg, 2, false);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("DA7212 init error: charge pump failed (0x%02X)\r\n", err);
        return err;
    }

    // 10) Mixer 설정
    const struct { uint8_t addr, val; } mixer_cfg[] = { {0x4B,0x08}, {0x4C,0x08} };
    for (size_t i = 0; i < sizeof(mixer_cfg)/sizeof(mixer_cfg[0]); ++i)
    {
        reg[0] = mixer_cfg[i].addr;
        reg[1] = mixer_cfg[i].val;
        err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, reg, 2, false);
        if (FSP_SUCCESS != err)
        {
            APP_ERR_PRINT("DA7212 init error: mixer reg 0x%02X failed (0x%02X)\r\n", reg[0], err);
            return err;
        }
    }

    // 11) Amp Gain 설정
    reg[0] = 0x4A; reg[1] = 0x30;
    err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, reg, 2, false);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("DA7212 init error: amp gain failed (0x%02X)\r\n", err);
        return err;
    }

    // 12) Amp Enable
    reg[0] = 0x6D; reg[1] = 0x80;
    err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, reg, 2, false);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("DA7212 init error: amp enable failed (0x%02X)\r\n", err);
        return err;
    }

    // 13) System Controller 활성화
    reg[0] = 0x51; reg[1] = 0xD9;
    err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, reg, 2, false);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("DA7212 init error: system ctrl failed (0x%02X)\r\n", err);
        return err;
    }

    // 14) 안정화 대기
    R_BSP_SoftwareDelay(250, BSP_DELAY_UNITS_MILLISECONDS);


    da7212_set_speaker_volume_max();
    da7212_set_dac_volume_max();


    return FSP_SUCCESS;
}

void i2c_master0_callback(i2c_master_callback_args_t * p_args)
{
    FSP_PARAMETER_NOT_USED(p_args);
}


static fsp_err_t da7212_read_register(uint8_t reg_addr, uint8_t* p_data)
{
    fsp_err_t err;

    err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, &reg_addr, 1, false);
    if (FSP_SUCCESS != err) return err;

    err = R_IIC_MASTER_Read(&g_i2c_master0_ctrl, p_data, 1, true);
    return err;
}


// LINE_AMP_GAIN (스피커 앰프) 최대 +15dB로 설정
fsp_err_t da7212_set_speaker_volume_max(void)
{
    uint8_t reg[2];
    fsp_err_t err;

    reg[0] = 0x4A; // LINE_AMP_GAIN
    reg[1] = 0x3F; // +15dB (0x00=-48dB, ... 0x3F=+15dB)
    err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, reg, 2, false);
    if (FSP_SUCCESS != err)
        APP_ERR_PRINT("Speaker volume set error: 0x%02X\n", err);

    return err;
}

// DAC 디지털 볼륨(좌우) 최대 +12dB
fsp_err_t da7212_set_dac_volume_max(void)
{
    uint8_t reg[2];
    fsp_err_t err;

    reg[0] = 0x45; reg[1] = 0x7F; // DAC_L_DIGITAL_GAIN: 0x7F = +12dB
    err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, reg, 2, false);
    if (FSP_SUCCESS != err) return err;

    reg[0] = 0x46; reg[1] = 0x7F; // DAC_R_DIGITAL_GAIN
    err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, reg, 2, false);

    return err;
}

