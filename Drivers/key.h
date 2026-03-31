#ifndef KEY_H_
#define KEY_H_

#include "config.h"

/*
 * 按键扫描模块。
 *
 * 提供非阻塞式按键扫描，支持消抖、长按和连发。
 * 模块只负责输出按键事件，不直接修改系统状态。
 */

#define KEY_DEBOUNCE_COUNT  10U
#define KEY_HOLD_START      200U
#define KEY_REPEAT_INTERVAL 40U

typedef enum {
    KEY_NONE = 0,
    KEY_UP,
    KEY_DOWN
} KeyEvent;

void key_init(void);
KeyEvent key_scan(void);

#endif
