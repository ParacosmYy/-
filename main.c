#include "config.h"
#include "display.h"
#include "freq.h"
#include "key.h"
#include "spwm.h"
#include "tick.h"

/*
 * Main foreground/background control flow.
 *
 * Interrupts generate timing and waveform output.
 * The foreground loop executes periodic application tasks on the 2 ms tick.
 */

#define APP_TICK_PERIOD_MS        2U
#define APP_KEY_SCAN_PERIOD_MS    20U
#define APP_KEY_SCAN_DIV          (APP_KEY_SCAN_PERIOD_MS / APP_TICK_PERIOD_MS)

static void App_HandleKeyEvent(KeyEvent evt)
{
    if (evt == KEY_UP) {
        Freq_Inc();
    } else if (evt == KEY_DOWN) {
        Freq_Dec();
    }
}

static void App_Task2ms(void)
{
    static unsigned char key_scan_div;

    Display_SetValue(Freq_Get());
    Display_Task();

    key_scan_div++;
    if (key_scan_div >= APP_KEY_SCAN_DIV) {
        key_scan_div = 0;
        App_HandleKeyEvent(Key_Scan());
    }
}

void main(void)
{
    SPWM_Init();
    Display_Init();
    Key_Init();

    EA = 1;

    SPWM_Start();
    Display_Start();

    while (1) {
        if (Tick_Get() != 0U) {
            Tick_Clear();
            App_Task2ms();
        }
    }
}
