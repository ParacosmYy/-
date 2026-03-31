#include "app.h"
#include "display.h"
#include "freq.h"
#include "key.h"
#include "tick.h"

/*
 * Application layer.
 *
 * Timer1 only provides the key-scan tick.
 * Display refresh stays in the main loop.
 */

#define APP_TICK_PERIOD_MS        2U
#define APP_KEY_SCAN_PERIOD_MS    20U
#define APP_KEY_SCAN_DIV          (APP_KEY_SCAN_PERIOD_MS / APP_TICK_PERIOD_MS)
#define APP_TIMER1_DIV            7U

static void app_handle_key_event(KeyEvent evt)
{
    if (evt == KEY_UP) {
        freq_increase();
        display_set_value(freq_get());
    } else if (evt == KEY_DOWN) {
        freq_decrease();
        display_set_value(freq_get());
    }
}

static void app_timer1_init(void)
{
    TMOD = (TMOD & 0x0F) | 0x20;
    TH1 = 0;
    TL1 = 0;

    ET1 = 1;
}

static void app_key_task(void)
{
    static unsigned char key_scan_div;

    key_scan_div++;
    if (key_scan_div >= APP_KEY_SCAN_DIV) {
        key_scan_div = 0U;
        app_handle_key_event(key_scan());
    }
}

void app_init(void)
{
    display_init();
    key_init();
    app_timer1_init();
}

void app_start(void)
{
    display_set_value(freq_get());
    TR1 = 1;
}

void app_task(void)
{
    if (tick_get() != 0U) {
        display_blank();
        tick_clear();
        app_key_task();
    }

    display_task();
}

void Timer1_ISR(void) interrupt 3 using 2
{
    static unsigned char tick_div;

    tick_div++;
    if (tick_div >= APP_TIMER1_DIV) {
        tick_div = 0U;
        tick_notify();
    }
}
