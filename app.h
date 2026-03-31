#ifndef APP_H_
#define APP_H_

#include "config.h"

/*
 * 应用层模块。
 *
 * 负责前后台任务调度、按键处理以及非 SPWM 的定时器资源管理。
 */

/* 初始化应用层相关模块 */
void App_Init(void);

/* 启动应用层运行 */
void App_Start(void);

/* 主循环中持续调用的应用任务入口 */
void App_Task(void);

#endif
