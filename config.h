#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <REGX52.H>

/*
 * 系统全局配置
 *
 * 集中定义: 晶振频率、引脚映射、系统级常量、全局变量声明.
 * 所有模块通过 #include "config.h" 引用, 做到改一处全局生效.
 */

/* ---- 晶振 ---- */
#define FOSC            11059200UL      /* 11.0592MHz */

/* ---- SPWM 配置 ---- */
#define SPWM_PIN        P3_7            /* PWM 波形输出引脚 */
#define SINE_POINTS     100             /* 每周期采样点数 */
#define PWM_LEVELS      100             /* PWM 分辨率 (0~99) */
#define MIN_PHASE_TICKS 80              /* 最小相位钳位 (机器周期), 防 ISR 溢出 */

/* ---- 频率范围 ---- */
#define FREQ_MIN        1               /* 最小频率 (Hz) */
#define FREQ_MAX        20              /* 最大频率 (Hz) */

/* ---- 数码管引脚 ---- */
#define SEG_PORT        P1              /* 段选端口 (a~g = P1.0~P1.6) */
#define DIG_TENS        P2_0            /* 十位位选 (低电平有效) */
#define DIG_ONES        P2_1            /* 个位位选 (低电平有效) */

/* ---- 数码管扫描 (Timer1 Mode 2 驱动) ---- */
#define DISP_TIMER_DIV  7               /* Timer1 分频: 7 × 278us ≈ 1.94ms ≈ 2ms */

/* ---- 按键引脚 ---- */
#define KEY_FREQ_UP     P3_3            /* K1: 频率+ */
#define KEY_FREQ_DOWN   P3_2            /* K2: 频率- */

/* ---- 按键消抖 / 长按 (主循环约 2ms/次) ---- */
#define KEY_DEBOUNCE_COUNT  10          /* 消抖阈值: 10 × 2ms ≈ 20ms */
#define KEY_HOLD_START      200         /* 长按触发延迟: 200 × 2ms ≈ 400ms */
#define KEY_REPEAT_INTERVAL 40          /* 长按重复间隔: 40 × 2ms ≈ 80ms */

/* ---- 全局变量 (在 main.c 中定义) ---- */
extern volatile unsigned char g_frequency;

/* ---- 系统节拍 (在 display.c Timer1 ISR 中置位) ---- */
extern volatile unsigned char g_tick;

#endif
