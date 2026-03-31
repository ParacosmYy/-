#include "display.h"
#include "tick.h"

/*
 * Display driver for a dual common-anode 7-segment module.
 *
 * Segment lines are shared on P1 and digit enables are on P2.0/P2.1.
 * Foreground scheduling keeps scan timing deterministic without placing
 * display work inside the interrupt service routine.
 */

#define DISPLAY_DIGIT_COUNT  2U
#define DISPLAY_BLANK_CODE   0xFFU
#define DISPLAY_TENS_INDEX   0U
#define DISPLAY_ONES_INDEX   1U

static const unsigned char code kSegCode[10] = {
    0xC0, 0xF9, 0xA4, 0xB0, 0x99,
    0x92, 0x82, 0xF8, 0x80, 0x90
};

typedef struct {
    unsigned char value;
    unsigned char digits[DISPLAY_DIGIT_COUNT];
    unsigned char scan_index;
} DisplayContext;

static DisplayContext s_display;

static void Display_AllOff(void)
{
    DIG_TENS = 1;
    DIG_ONES = 1;
}

static void Display_UpdateDigits(unsigned char value)
{
    s_display.value = value;

    if (value >= 10U) {
        s_display.digits[DISPLAY_TENS_INDEX] = value / 10U;
    } else {
        s_display.digits[DISPLAY_TENS_INDEX] = DISPLAY_BLANK_CODE;
    }

    s_display.digits[DISPLAY_ONES_INDEX] = value % 10U;
}

static void Display_OutputDigit(unsigned char index)
{
    unsigned char digit;

    digit = s_display.digits[index];

    Display_AllOff();

    if (digit <= 9U) {
        SEG_PORT = kSegCode[digit];
    } else {
        SEG_PORT = DISPLAY_BLANK_CODE;
    }

    if (index == DISPLAY_TENS_INDEX) {
        DIG_TENS = 0;
    } else {
        DIG_ONES = 0;
    }
}

void Display_SetValue(unsigned char value)
{
    if (value < FREQ_MIN) {
        value = FREQ_MIN;
    } else if (value > FREQ_MAX) {
        value = FREQ_MAX;
    }

    Display_UpdateDigits(value);
}

void Display_Task(void)
{
    Display_OutputDigit(s_display.scan_index);

    s_display.scan_index++;
    if (s_display.scan_index >= DISPLAY_DIGIT_COUNT) {
        s_display.scan_index = 0;
    }
}

void Timer1_ISR(void) interrupt 3 using 2
{
    static unsigned char div;

    div++;
    if (div >= DISP_TIMER_DIV) {
        div = 0;
        Tick_Notify();
    }
}

void Display_Init(void)
{
    SEG_PORT = DISPLAY_BLANK_CODE;
    Display_AllOff();

    s_display.scan_index = 0;
    Display_UpdateDigits(FREQ_MIN);

    TMOD = (TMOD & 0x0F) | 0x20;
    TH1 = 0;
    TL1 = 0;

    ET1 = 1;
}

void Display_Start(void)
{
    TR1 = 1;
}
