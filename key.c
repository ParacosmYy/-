#include "key.h"

/*
 * 按键扫描模块 — 状态机实现
 *
 * 状态转移:
 *   IDLE  ──按下消抖──▶ HELD   (触发一次事件)
 *   HELD  ──持续按住──▶ REPEAT (触发长按事件)
 *   REPEAT ──持续按住──▶ REPEAT (每隔 REPEAT_INTERVAL 触发一次)
 *   任意状态 ──松开──▶ IDLE
 *
 * 只负责检测按键事件并返回, 不直接修改系统状态.
 */

/* ---- 按键状态 ---- */
#define STATE_IDLE      0       /* 空闲: 等待按下 */
#define STATE_HELD      1       /* 保持: 等待长按阈值 */
#define STATE_REPEAT    2       /* 重复: 持续触发 */

static unsigned char k1_state;
static unsigned char k1_count;
static unsigned char k2_state;
static unsigned char k2_count;

/* ---- 初始化 ---- */
void Key_Init(void)
{
    k1_state = STATE_IDLE;
    k1_count = 0;
    k2_state = STATE_IDLE;
    k2_count = 0;
}

/* ---- 按键扫描 (主循环中调用, 非阻塞) ---- */
KeyEvent Key_Scan(void)
{
    /* ---- K1: 频率增加 ---- */
    if (KEY_FREQ_UP == 0) {
        k1_count++;
        switch (k1_state) {
        case STATE_IDLE:
            if (k1_count >= KEY_DEBOUNCE_COUNT) {
                k1_state = STATE_HELD;
                k1_count = 0;
                return KEY_UP;
            }
            break;
        case STATE_HELD:
            if (k1_count >= KEY_HOLD_START) {
                k1_state = STATE_REPEAT;
                k1_count = 0;
                return KEY_UP;
            }
            break;
        case STATE_REPEAT:
            if (k1_count >= KEY_REPEAT_INTERVAL) {
                k1_count = 0;
                return KEY_UP;
            }
            break;
        }
    } else {
        k1_state = STATE_IDLE;
        k1_count = 0;
    }

    /* ---- K2: 频率减少 ---- */
    if (KEY_FREQ_DOWN == 0) {
        k2_count++;
        switch (k2_state) {
        case STATE_IDLE:
            if (k2_count >= KEY_DEBOUNCE_COUNT) {
                k2_state = STATE_HELD;
                k2_count = 0;
                return KEY_DOWN;
            }
            break;
        case STATE_HELD:
            if (k2_count >= KEY_HOLD_START) {
                k2_state = STATE_REPEAT;
                k2_count = 0;
                return KEY_DOWN;
            }
            break;
        case STATE_REPEAT:
            if (k2_count >= KEY_REPEAT_INTERVAL) {
                k2_count = 0;
                return KEY_DOWN;
            }
            break;
        }
    } else {
        k2_state = STATE_IDLE;
        k2_count = 0;
    }

    return KEY_NONE;
}
