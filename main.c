#include "config.h"
#include "spwm.h"
#include "display.h"
#include "key.h"

/*
 * 基于 ST89C52 的 1Hz~20Hz 可调 SPWM 波形发生器
 *
 * 功能:
 *   - P3.7 输出 SPWM 波形 (查表法 + Timer0 中断)
 *   - K1(P3.3) 频率+, K2(P3.2) 频率-
 *   - 两位共阳数码管实时显示频率 (P1 段选, P2.0/P2.1 位选)
 *
 * 晶振: 11.0592MHz
 */

/* ---- 全局变量 ---- */
volatile unsigned char g_frequency = 1;  /* 当前输出频率, 初始 1Hz */

void main(void)
{
    SPWM_Init();
    Display_Init();
    Key_Init();

    while (1) {
        Display_Refresh();    /* 动态扫描数码管 */

        /* 按键事件处理: 由主循环统一协调频率修改 */
        {
            KeyEvent evt = Key_Scan();
            if (evt == KEY_UP) {
                if (g_frequency < FREQ_MAX) {
                    g_frequency++;
                }
            } else if (evt == KEY_DOWN) {
                if (g_frequency > FREQ_MIN) {
                    g_frequency--;
                }
            }
        }
    }
}
