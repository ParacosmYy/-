#include "key.h"

/*
 * 按键扫描模块实现。
 *
 * 每个按键使用独立状态机。
 * 检测到有效按下、长按起始或重复触发时返回对应事件。
 */

#define KEY_STATE_IDLE      0U
#define KEY_STATE_HELD      1U
#define KEY_STATE_REPEAT    2U

typedef struct {
    unsigned char state;
    unsigned char count;
} KeyContext;

static KeyContext s_key_up;
static KeyContext s_key_down;

static KeyEvent Key_Process(KeyContext *ctx, unsigned char pressed, KeyEvent evt)
{
    if (pressed != 0U) {
        ctx->count++;

        switch (ctx->state) {
        case KEY_STATE_IDLE:
            if (ctx->count >= KEY_DEBOUNCE_COUNT) {
                ctx->state = KEY_STATE_HELD;
                ctx->count = 0U;
                return evt;
            }
            break;

        case KEY_STATE_HELD:
            if (ctx->count >= KEY_HOLD_START) {
                ctx->state = KEY_STATE_REPEAT;
                ctx->count = 0U;
                return evt;
            }
            break;

        case KEY_STATE_REPEAT:
            if (ctx->count >= KEY_REPEAT_INTERVAL) {
                ctx->count = 0U;
                return evt;
            }
            break;

        default:
            ctx->state = KEY_STATE_IDLE;
            ctx->count = 0U;
            break;
        }
    } else {
        ctx->state = KEY_STATE_IDLE;
        ctx->count = 0U;
    }

    return KEY_NONE;
}

void Key_Init(void)
{
    s_key_up.state = KEY_STATE_IDLE;
    s_key_up.count = 0U;
    s_key_down.state = KEY_STATE_IDLE;
    s_key_down.count = 0U;
}

KeyEvent Key_Scan(void)
{
    KeyEvent evt;

    evt = Key_Process(&s_key_up, (KEY_FREQ_UP == 0U), KEY_UP);
    if (evt != KEY_NONE) {
        return evt;
    }

    return Key_Process(&s_key_down, (KEY_FREQ_DOWN == 0U), KEY_DOWN);
}
