#include "config.h"
#include "spwm.h"
#include "display.h"
#include "key.h"

/*
 * 基于 ST89C52 的 1Hz~20Hz 可调 SPWM 波形发生器
 主控制.
 *
 * 功能:
 *   - P3.7 输出 SPWM 波形 (查表法 + Timer0 中断)
 *   - K1(P3.3) 频率+, K2(P3.2) 频率- (支持长按连加/连减)
 *   - 两位共阳数码管实时显示频率 (Timer1 中断驱动扫描)
 *   - 频率 1~9 时十位自动消隐, 10~20 正常显示
 *
 * 晶振: 11.0592MHz
 */

/* ---- 全局变量 ---- */
volatile unsigned char g_frequency = 1;  /* 当前输出频率, 初始 1Hz */

void main(void)
{
    KeyEvent evt;

    SPWM_Init();
    Display_Init();
    Key_Init();

    EA = 1;     /* 所有外设初始化完成后再开全局中断 */

    while (1) {
        if (g_tick) {
            g_tick = 0;

            evt = Key_Scan();
            if (evt == KEY_UP) {
                if (g_frequency < FREQ_MAX) {
                    g_frequency++;
                }
            } else if (evt == KEY_DOWN) {
                if (g_frequency > FREQ_MIN) {
                    g_frequency--;
                }
            }
        } else {
            PCON |= 0x01;   /* IDLE 模式, 任何中断自动唤醒 */
        }
    }
}
