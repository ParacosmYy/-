#include "display.h"

/*
 * 数码管显示模块实现。
 *
 * 段选信号使用 P1，位选信号使用 P2.0 和 P2.1。
 * 主循环持续调用 display_task()，每次只刷新一位，以实现动态显示。
 */

#define DISPLAY_DIGIT_COUNT  2U
#define DISPLAY_BLANK_CODE   0xFFU
#define DISPLAY_TENS_INDEX   0U
#define DISPLAY_ONES_INDEX   1U

static const unsigned char code display_seg_code[10] = {
    0xC0, 0xF9, 0xA4, 0xB0, 0x99,
    0x92, 0x82, 0xF8, 0x80, 0x90
};

typedef struct {
    unsigned char digits[DISPLAY_DIGIT_COUNT];
    unsigned char scan_index;
} display_ctx_t;

static display_ctx_t s_display_ctx;

static void display_all_off(void)
{
    DIG_TENS = 1;
    DIG_ONES = 1;
}

static void display_update_digits(unsigned char value)
{
    if (value >= 10U) {
        s_display_ctx.digits[DISPLAY_TENS_INDEX] = value / 10U;
    } else {
        s_display_ctx.digits[DISPLAY_TENS_INDEX] = DISPLAY_BLANK_CODE;
    }

    s_display_ctx.digits[DISPLAY_ONES_INDEX] = value % 10U;
}

static void display_output_digit(unsigned char index)
{
    unsigned char digit;

    digit = s_display_ctx.digits[index];

    display_all_off();

    if (digit <= 9U) {
        SEG_PORT = display_seg_code[digit];
    } else {
        SEG_PORT = DISPLAY_BLANK_CODE;
    }

    if (index == DISPLAY_TENS_INDEX) {
        DIG_TENS = 0;
    } else {
        DIG_ONES = 0;
    }
}

void display_init(void)
{
    SEG_PORT = DISPLAY_BLANK_CODE;
    display_all_off();

    s_display_ctx.scan_index = 0U;
    display_update_digits(FREQ_MIN);
}

void display_set_value(unsigned char value)
{
    if (value < FREQ_MIN) {
        value = FREQ_MIN;
    } else if (value > FREQ_MAX) {
        value = FREQ_MAX;
    }

    display_update_digits(value);
}

void display_blank(void)
{
    display_all_off();
}

void display_task(void)
{
    display_output_digit(s_display_ctx.scan_index);

    s_display_ctx.scan_index++;
    if (s_display_ctx.scan_index >= DISPLAY_DIGIT_COUNT) {
        s_display_ctx.scan_index = 0U;
    }
}
