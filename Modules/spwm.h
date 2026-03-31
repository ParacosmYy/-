#ifndef SPWM_H_
#define SPWM_H_

#include "config.h"

/*
 * SPWM 输出模块。
 *
 * 使用 Timer0 中断输出 SPWM 波形。
 * 模块仅负责波形生成，不处理按键和显示逻辑。
 */

/* 每个正弦周期的采样点数 */
#define SINE_POINTS     100U

/* PWM 分辨率 */
#define PWM_LEVELS      256U

/* 单相最小计数值，防止定时过短 */
#define MIN_PHASE_TICKS 100U

/* 初始化 SPWM 模块 */
void spwm_init(void);

/* 启动 SPWM 输出 */
void spwm_start(void);

#endif
