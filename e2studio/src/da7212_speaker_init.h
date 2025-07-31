/* da7212_speaker_init.h */
#ifndef DA7212_SPEAKER_INIT_H_
#define DA7212_SPEAKER_INIT_H_

#include "bsp_api.h"  // fsp_err_t 정의

/**
 * @brief   DA7212 사운드 출력용 초기화
 *
 * EK-RA8P1 보드에서 DA7212 오디오 코덱을 스피커 드라이버로 사용하기 위한
 * I2C 레지스터 초기화 함수를 제공합니다.
 *
 * @note    Smart Configurator에서 다음을 설정해야 합니다:
 *          - g_i2c_master0_cfg.slave_address = 0x1A
 *          - Peripherals: I2C Master (IIC0), SSI (r_ssi), GPT (r_gpt)
 */


fsp_err_t da7212_speaker_init(void);

#endif // DA7212_SPEAKER_INIT_H_
