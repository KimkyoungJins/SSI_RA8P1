/* =============================================================
 *  da7212_speaker_init.h  –  EK‑RA8P1 / DA7212 Speaker Driver
 *  Revised: 2025‑07‑31  (all fixes applied)
 * =============================================================*/
#ifndef DA7212_SPEAKER_INIT_H_
#define DA7212_SPEAKER_INIT_H_

#include "bsp_api.h"   /* fsp_err_t, delay units */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Initialise DA7212 codec for differential speaker output (SP_P / SP_N).
 *         – 48 kHz, 16‑bit, I²S, MCU master clocking.
 * @return FSP_SUCCESS on success, otherwise FSP error code.
 */
fsp_err_t da7212_speaker_init(void);

#ifdef __cplusplus
}
#endif

#endif /* DA7212_SPEAKER_INIT_H_ */
