#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <REGX52.H>

/*
 * 两位共阳七段数码管显示模块
 * 动态扫描方式, P1 段选, P2.0/P2.1 位选
 */

/* 数码管引脚定义 */
#define SEG_PORT        P1      /* 段选端口 (a~g 对应 P1.0~P1.6) */
#define DIG_TENS        P2_0    /* 十位位选 (低电平有效) */
#define DIG_ONES        P2_1    /* 个位位选 (低电平有效) */

/* 动态扫描每位显示时间 (ms) */
#define DISP_SCAN_MS    2

/* 外部全局变量 */
extern unsigned char g_frequency;

void Display_Init(void);
void Display_Refresh(void);

#endif
