#ifndef __KEY_H__
#define __KEY_H__

#include "config.h"

/*
 * 按键输入模块
 * K1: 频率增加, K2: 频率减少
 * 非阻塞计数消抖, 返回按键事件, 由主循环处理.
 */

/* 按键事件类型 */
typedef enum {
    KEY_NONE,       /* 无按键事件 */
    KEY_UP,         /* K1: 频率增加 */
    KEY_DOWN        /* K2: 频率减少 */
} KeyEvent;

void Key_Init(void);
KeyEvent Key_Scan(void);

#endif
