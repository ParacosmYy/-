#include "app.h"
#include "display.h"
#include "freq.h"
#include "key.h"
#include "tick.h"

/*
 * 应用层实现。
 *
 * Timer1 仅用于产生按键扫描节拍。
 * 数码管刷新始终在主循环中完成，不由中断驱动。
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

static void App_Timer1Init(void)
{
    TMOD = (TMOD & 0x0F) | 0x20;
    TH1 = 0;
    TL1 = 0;

    ET1 = 1;
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

void App_Init(void)
{
    Display_Init();
    Key_Init();
    App_Timer1Init();
}

void App_Start(void)
{
    Display_SetValue(Freq_Get());
    TR1 = 1;
}

void App_Task(void)
{
    Display_Task();

    if (Tick_Get() != 0U) {
        Tick_Clear();
        App_KeyTask();
        Display_SetValue(Freq_Get());
    }
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
