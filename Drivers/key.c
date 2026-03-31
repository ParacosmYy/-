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
} key_ctx_t;

static key_ctx_t s_key_up_ctx;
static key_ctx_t s_key_down_ctx;

static KeyEvent key_process(key_ctx_t *ctx, unsigned char pressed, KeyEvent evt)
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

void key_init(void)
{
    s_key_up_ctx.state = KEY_STATE_IDLE;
    s_key_up_ctx.count = 0U;
    s_key_down_ctx.state = KEY_STATE_IDLE;
    s_key_down_ctx.count = 0U;
}

KeyEvent key_scan(void)
{
    KeyEvent evt;

    evt = key_process(&s_key_up_ctx, (KEY_FREQ_UP == 0U), KEY_UP);
    if (evt != KEY_NONE) {
        return evt;
    }

    return key_process(&s_key_down_ctx, (KEY_FREQ_DOWN == 0U), KEY_DOWN);
}
