#include "key.h"
#include "spwm.h"

/*
 * 按键扫描模块
 *
 * 按键接在 P3.2 / P3.3, 低电平有效.
 * 非阻塞计数消抖, 主循环每次调用约 2ms.
 * 频率范围限制在 FREQ_MIN ~ FREQ_MAX (1~20Hz).
 */

/* 外部全局变量 */
extern volatile unsigned char g_frequency;

/* 按键消抖状态 */
static unsigned char k1_count;      /* K1 消抖计数器 */
static unsigned char k1_triggered;  /* K1 已触发标志 (防止长按重复) */
static unsigned char k2_count;
static unsigned char k2_triggered;

/* ---- 初始化 ---- */
void Key_Init(void)
{
    k1_count = 0;
    k1_triggered = 0;
    k2_count = 0;
    k2_triggered = 0;
}

/* ---- 按键扫描 (主循环中调用, 非阻塞) ---- */
void Key_Scan(void)
{
    /* ---- K1: 频率增加 ---- */
    if (KEY_FREQ_UP == 0) {
        if (!k1_triggered) {
            k1_count++;
            if (k1_count >= KEY_DEBOUNCE_COUNT) {
                k1_triggered = 1;
                if (g_frequency < FREQ_MAX) {
                    g_frequency++;
                    SPWM_UpdateFreq();
                }
            }
        }
    } else {
        k1_count = 0;
        k1_triggered = 0;
    }

    /* ---- K2: 频率减少 ---- */
    if (KEY_FREQ_DOWN == 0) {
        if (!k2_triggered) {
            k2_count++;
            if (k2_count >= KEY_DEBOUNCE_COUNT) {
                k2_triggered = 1;
                if (g_frequency > FREQ_MIN) {
                    g_frequency--;
                    SPWM_UpdateFreq();
                }
            }
        }
    } else {
        k2_count = 0;
        k2_triggered = 0;
    }
}
