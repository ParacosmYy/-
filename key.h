#ifndef KEY_H_
#define KEY_H_

#include "config.h"

/*
 * 按键输入模块
 * K1: 频率增加, K2: 频率减少
 * 非阻塞计数消抖 + 长按连按, 返回按键事件, 由主循环处理.
 */

/* ---- 按键时序参数 (主循环约 2ms/次) ---- */
#define KEY_DEBOUNCE_COUNT  10          /* 消抖阈值: 10 × 2ms ≈ 20ms */
#define KEY_HOLD_START      200         /* 长按延迟: 200 × 2ms ≈ 400ms */
#define KEY_REPEAT_INTERVAL 40          /* 重复间隔: 40 × 2ms ≈ 80ms */

/* ---- 按键事件类型 ---- */
typedef enum {
    KEY_NONE,       /* 无事件 */
    KEY_UP,         /* K1: 频率增加 */
    KEY_DOWN        /* K2: 频率减少 */
} KeyEvent;

void Key_Init(void);
KeyEvent Key_Scan(void);

#endif
