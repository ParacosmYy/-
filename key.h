#ifndef KEY_H_
#define KEY_H_

#include "config.h"

/*
 * 按键扫描模块。
 *
 * 提供非阻塞式按键扫描，支持消抖、长按和连发。
 * 模块只负责输出按键事件，不直接修改系统状态。
 */

/* 按键时序参数，基于约 2 ms 的扫描节拍 */
#define KEY_DEBOUNCE_COUNT  10U
#define KEY_HOLD_START      200U
#define KEY_REPEAT_INTERVAL 40U

/* 按键事件类型 */
typedef enum {
    KEY_NONE = 0,
    KEY_UP,
    KEY_DOWN
} KeyEvent;

/* 初始化按键状态机 */
void Key_Init(void);

/* 扫描按键并返回事件 */
KeyEvent Key_Scan(void);

#endif
