#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "config.h"

/*
 * 数码管显示模块。
 *
 * 负责两位共阳数码管的前台轮询刷新。
 * 显示刷新仅在主循环中执行，不依赖定时器中断完成动态扫描。
 */

/* 初始化数码管显示模块 */
void Display_Init(void);

/* 设置待显示的频率值 */
void Display_SetValue(unsigned char value);

/* 执行一次显示扫描任务，每次仅刷新一位 */
void Display_Task(void);

#endif
