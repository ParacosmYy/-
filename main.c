#include "config.h"
#include "display.h"
#include "freq.h"
#include "key.h"
#include "spwm.h"
#include "tick.h"

/*
 * Main foreground/background control flow.
 *
 * Timer0 generates SPWM.
 * Timer1 is retained only as a coarse system tick for key scanning.
 * Display refresh is handled exclusively in the foreground while-loop.
 */

#define APP_TICK_PERIOD_MS        2U
#define APP_KEY_SCAN_PERIOD_MS    20U
#define APP_KEY_SCAN_DIV          (APP_KEY_SCAN_PERIOD_MS / APP_TICK_PERIOD_MS)
#define APP_TIMER1_DIV            7U

static void App_HandleKeyEvent(KeyEvent evt)
{
    if (evt == KEY_UP) {
        Freq_Inc();
    } else if (evt == KEY_DOWN) {
        Freq_Dec();
    }
}

static void App_KeyTask(void)
{
    static unsigned char key_scan_div;

    key_scan_div++;
    if (key_scan_div >= APP_KEY_SCAN_DIV) {
        key_scan_div = 0U;
        App_HandleKeyEvent(Key_Scan());
    }
}

static void App_TickInit(void)
{
    TMOD = (TMOD & 0x0F) | 0x20;
    TH1 = 0;
    TL1 = 0;

    ET1 = 1;
    TR1 = 1;
}

void Timer1_ISR(void) interrupt 3 using 2
{
    static unsigned char tick_div;

    tick_div++;
    if (tick_div >= APP_TIMER1_DIV) {
        tick_div = 0U;
        Tick_Notify();
    }
}

void main(void)
{
    SPWM_Init();
    Display_Init();
    Key_Init();
    App_TickInit();

    EA = 1;

    SPWM_Start();
    Display_SetValue(Freq_Get());

    while (1) {
        Display_Task();

        if (Tick_Get() != 0U) {
            Tick_Clear();
            App_KeyTask();
            Display_SetValue(Freq_Get());
        }
    }
}
