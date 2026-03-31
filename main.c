#include "config.h"
#include "freq.h"
#include "tick.h"
#include "spwm.h"
#include "display.h"
#include "key.h"

/*
 * 基于 ST89C52 的 1Hz~20Hz 可调 SPWM 波形发生器 — 主控制
 *
 * 功能:
 *   - P3.7 输出 SPWM 波形 (查表法 + Timer0 中断)
 *   - K1(P3.3) 频率+, K2(P3.2) 频率- (支持长按连加/连减)
 *   - 两位共阳数码管实时显示频率 (Timer1 中断驱动扫描)
 *   - 频率 1~9 时十位自动消隐, 10~20 正常显示
 *
 * 晶振: 11.0592MHz
 */

void main(void)
{
    KeyEvent evt;

    SPWM_Init();
    Display_Init();
    Key_Init();

    EA = 1;     /* 开全局中断 */

    SPWM_Start();       /* Timer0 启动, ISR 开始输出波形 */
    Display_Start();    /* Timer1 启动, ISR 开始扫描数码管 */

    while (1) {
        if (Tick_Get()) {
            Tick_Clear();

            evt = Key_Scan();
            if (evt == KEY_UP) {
                Freq_Inc();
            } else if (evt == KEY_DOWN) {
                Freq_Dec();
            }
        }
    }
}
