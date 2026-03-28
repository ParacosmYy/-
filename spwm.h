#ifndef __SPWM_H__
#define __SPWM_H__

#include <REGX52.H>

/*
 * SPWM 波形发生器模块
 * 使用 Timer0 Mode1 (16位) 双相中断生成 SPWM
 */

/* SPWM 输出引脚 */
#define SPWM_PIN        P3_7

/* 正弦表参数 */
#define SINE_POINTS     100     /* 每周期采样点数 */
#define PWM_LEVELS      100     /* PWM 分辨率 (0~99) */

/* 最小相位钳位 (机器周期数), 防止 ISR 执行不完 */
#define MIN_PHASE_TICKS 80

/* 频率范围 */
#define FREQ_MIN        1
#define FREQ_MAX        20

/* 外部全局变量 (在 main.c 中定义) */
extern unsigned char g_frequency;

void SPWM_Init(void);
void SPWM_UpdateFreq(void);

#endif
