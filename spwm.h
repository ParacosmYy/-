#ifndef SPWM_H_
#define SPWM_H_

#include "config.h"

/*
 * SPWM 波形发生器模块
 * 使用 Timer0 Mode1 (16位) 双相中断生成 SPWM
 */

/* ---- SPWM 算法参数 (统一定义在 config.h) ---- */

void SPWM_Init(void);

/* ---- SPWM 算法参数 ---- */
#define SINE_POINTS     100             /* 每周期采样点数 */
#define PWM_LEVELS      100             /* PWM 分辨率 (0~99) */
#define MIN_PHASE_TICKS 80              /* 最小相位钳位 (机器周期), 防 ISR 抵溢出 */
#endif
