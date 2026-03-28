#include "key.h"
#include "delay.h"
#include "spwm.h"

/*
 * 按键扫描模块
 *
 * 按键接在 P3.2 / P3.3, 低电平有效.
 * 采用软件延时消抖, 每次按下调节 1Hz.
 * 频率范围限制在 FREQ_MIN ~ FREQ_MAX (1~20Hz).
 */

/* 外部全局变量 */
extern unsigned char g_frequency;

/* ---- 初始化 ---- */
void Key_Init(void)
{
    /*
     * 8051 准双向口默认输出高电平, 可直接作为按键输入.
     * 外部上拉电阻 + 按键接地, 按下时为低电平.
     */
}

/* ---- 按键扫描 (主循环中调用) ---- */
void Key_Scan(void)
{
    /* ---- K1: 频率增加 ---- */
    if (KEY_FREQ_UP == 0) {
        Delay_ms(KEY_DEBOUNCE_MS);
        if (KEY_FREQ_UP == 0) {
            if (g_frequency < FREQ_MAX) {
                g_frequency++;
                SPWM_UpdateFreq();
            }
            /* 等待按键释放 */
            while (KEY_FREQ_UP == 0);
            Delay_ms(KEY_DEBOUNCE_MS);
        }
    }

    /* ---- K2: 频率减少 ---- */
    if (KEY_FREQ_DOWN == 0) {
        Delay_ms(KEY_DEBOUNCE_MS);
        if (KEY_FREQ_DOWN == 0) {
            if (g_frequency > FREQ_MIN) {
                g_frequency--;
                SPWM_UpdateFreq();
            }
            /* 等待按键释放 */
            while (KEY_FREQ_DOWN == 0);
            Delay_ms(KEY_DEBOUNCE_MS);
        }
    }
}
