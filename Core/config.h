#ifndef CONFIG_H_
#define CONFIG_H_

#include <REGX52.H>

/*
 * 板级硬件配置文件。
 *
 * 统一定义系统晶振、引脚映射以及跨模块共享常量。
 * 业务状态不在本文件中保存，只提供硬件相关配置。
 */

/* 系统晶振频率 */
#define FOSC            11059200UL

/* 频率范围，单位：Hz */
#define FREQ_MIN        1U
#define FREQ_MAX        20U

/* SPWM 输出引脚 */
#define SPWM_PIN        P3_7

/* 数码管接口定义 */
#define SEG_PORT        P1
#define DIG_TENS        P2_0
#define DIG_ONES        P2_1

/* 按键接口定义 */
#define KEY_FREQ_UP     P3_3
#define KEY_FREQ_DOWN   P3_2

#endif
