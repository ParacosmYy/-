#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "config.h"

/*
 * 两位共阳七段数码管显示模块
 * Timer1 Mode 2 中断驱动动态扫描, 非阻塞.
 * P1 段选, P2.0/P2.1 位选.
 */

void Display_Init(void);

#endif
