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

/* ---- 单键状态上下文 ---- */
typedef struct {
    unsigned char state;
    unsigned char count;
} KeyCtx;

static KeyCtx k1;
static KeyCtx k2;

/* ---- 初始化 ---- */
void Key_Init(void)
{
    k1.state = STATE_IDLE; k1.count = 0;
    k2.state = STATE_IDLE; k2.count = 0;
}

/* ---- 单键状态机 (内部) ---- */
static KeyEvent key_process(KeyCtx *ctx, unsigned char pressed, KeyEvent evt)
{
    if (pressed) {
        ctx->count++;
        switch (ctx->state) {
        case STATE_IDLE:
            if (ctx->count >= KEY_DEBOUNCE_COUNT) {
                ctx->state = STATE_HELD;
                ctx->count = 0;
                return evt;
            }
            break;
        case STATE_HELD:
            if (ctx->count >= KEY_HOLD_START) {
                ctx->state = STATE_REPEAT;
                ctx->count = 0;
                return evt;
            }
            break;
        case STATE_REPEAT:
            if (ctx->count >= KEY_REPEAT_INTERVAL) {
                ctx->count = 0;
                return evt;
            }
            break;
        }
    } else {
        ctx->state = STATE_IDLE;
        ctx->count = 0;
    }
    return KEY_NONE;
}

/* ---- 按键扫描 (主循环中调用, 非阻塞) ---- */
KeyEvent Key_Scan(void)
{
    KeyEvent evt;

    evt = key_process(&k1, KEY_FREQ_UP == 0, KEY_UP);
    if (evt != KEY_NONE) return evt;

    return key_process(&k2, KEY_FREQ_DOWN == 0, KEY_DOWN);
}
