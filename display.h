#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "config.h"

/*
 * 两位共阳七段数码管显示模块
 * Timer1 Mode 2 中断驱动动态扫描, 非阻塞.
 * P1 段选, P2.0/P2.1 位选.
 */

/* ---- 显示扫描参数 ---- */
#define DISP_TIMER_DIV  7               /* Timer1 分频: 7 × 278us ≈ 1.94ms ≈ 2ms */

void Display_Init(void);
void Display_Start(void);

#endif
