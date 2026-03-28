#ifndef CONFIG_H_
#define CONFIG_H_

#include <REGX52.H>

/*
 * 板级硬件配置
 *
 * 只集中定义: 晶振、引脚映射、跨模块共享常量和全局变量声明.
 * 各模块的算法参数 (采样点数、消抖阈值等) 在各自 .h 中定义.
 */

/* ---- 晶振 ---- */
#define FOSC            11059200UL      /* 11.0592MHz */

/* ---- 频率范围 (key / spwm / main 共享) ---- */
#define FREQ_MIN        1               /* 最小频率 (Hz) */
#define FREQ_MAX        20              /* 最大频率 (Hz) */

/* ---- SPWM 引脚 ---- */
#define SPWM_PIN        P3_7            /* PWM 波形输出引脚 */

/* ---- 数码管引脚 ---- */
#define SEG_PORT        P1              /* 段选端口 (a~g = P1.0~P1.6) */
#define DIG_TENS        P2_0            /* 十位位选 (低电平有效) */
#define DIG_ONES        P2_1            /* 个位位选 (低电平有效) */

/* ---- 按键引脚 ---- */
#define KEY_FREQ_UP     P3_3            /* K1: 频率+ */
#define KEY_FREQ_DOWN   P3_2            /* K2: 频率- */

/* ---- 全局变量 (在 main.c 中定义) ---- */
extern volatile unsigned char g_frequency;

#endif
