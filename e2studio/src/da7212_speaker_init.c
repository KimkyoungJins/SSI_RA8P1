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

/**
 * DA7212 레지스터 초기
 * @return
 */


fsp_err_t da7212_speaker_init(void)
{
    fsp_err_t err;
    uint8_t   reg[2];

    R_BSP_SoftwareDelay(50, BSP_DELAY_UNITS_MILLISECONDS);

    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_05_PIN_12, BSP_IO_LEVEL_HIGH);

    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_05_PIN_11, BSP_IO_LEVEL_HIGH);

    R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);



    // 1) I2C 마스터 오픈 (slave_address: 0x1A)
    err = R_IIC_MASTER_Open(&g_i2c_master0_ctrl, &g_i2c_master0_cfg);
    if (FSP_SUCCESS != err)

    {
        SEGGER_RTT_printf(0, "\r\n");
        APP_ERR_PRINT("DA7212 init error: I2C open failed (0x%02X)\r\n", err);
        return err;
    }

    // 빈 콜백 함수 추가하기
    /* ―― ① 콜백 등록 추가 ――――――――――――――――――――― */
    R_IIC_MASTER_CallbackSet(&g_i2c_master0_ctrl,
                             i2c_master0_callback,   /* 아래 정의한 콜백 */
                             NULL,                   /* p_context */
                             NULL);                  /* p_callback_memory */
    /* ――――――――――――――――――――――――――――――――――――― */

    // 2) SYSTEM_ACTIVE 설정
    reg[0] = 0xFD; reg[1] = 0x01;
    err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, reg, 2, false);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("DA7212 init error: SYSTEM_ACTIVE write failed (0x%02X)\r\n", err);
        return err;
    }
    R_BSP_SoftwareDelay(40, BSP_DELAY_UNITS_MILLISECONDS);

    // 3) BIAS_EN 활성화
    reg[0] = 0x23; reg[1] = 0x08;
    err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, reg, 2, false);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("DA7212 init error: BIAS_EN write failed (0x%02X)\r\n", err);
        return err;
    }
    R_BSP_SoftwareDelay(25, BSP_DELAY_UNITS_MILLISECONDS);

    // 4) 샘플레이트 48kHz 설정
    reg[0] = 0x22; reg[1] = 0x0B;
    err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, reg, 2, false);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("DA7212 init error: sample rate write failed (0x%02X)\r\n", err);
        return err;
    }

    // 5) PLL 설정
    const struct { uint8_t addr, val; } pll_cfg[] = { {0x27,0x84}, {0x24,0x00}, {0x25, 0x00}, {0x26,0x20} };
    for (size_t i = 0; i < sizeof(pll_cfg)/sizeof(pll_cfg[0]); ++i)
    {
        reg[0] = pll_cfg[i].addr;
        reg[1] = pll_cfg[i].val;
        err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, reg, 2, false);
        if (FSP_SUCCESS != err)
        {
            APP_ERR_PRINT("DA7212 init error: PLL reg 0x%02X write failed (0x%02X)\r\n", reg[0], err);
            return err;
        }
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

    return FSP_SUCCESS;
}

void i2c_master0_callback(i2c_master_callback_args_t * p_args)
{
    /* 지금은 아무 것도 하지 않음 */
    FSP_PARAMETER_NOT_USED(p_args);
}
