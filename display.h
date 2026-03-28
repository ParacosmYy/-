#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "config.h"

/*
 * 两位共阳七段数码管显示模块
 * 动态扫描方式, P1 段选, P2.0/P2.1 位选
 */

void Display_Init(void);
void Display_Refresh(void);

#endif
