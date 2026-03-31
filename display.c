#include "display.h"
#include "freq.h"
#include "tick.h"

/*
 * 数码管显示模块 — Timer1 中断驱动
 *
 * Timer1 Mode 2 (8位自动重装), 重装值 0 → 溢出周期 256 × 1.085us ≈ 278us.
 * 软件分频 DISP_TIMER_DIV = 7 → 显示扫描周期 ≈ 1.94ms ≈ 2ms.
 * 每次扫描切换一位数码管, 同时通过 Tick_Notify() 通知主循环.
 *
 * 功能: 频率 1~9 时十位消隐, 10~20 正常显示.
 */

/* ---- 共阳七段码查找表 (0~9) ---- */
const unsigned char code seg_table[10] = {
    0xC0,  /* 0 */
    0xF9,  /* 1 */
    0xA4,  /* 2 */
    0xB0,  /* 3 */
    0x99,  /* 4 */
    0x92,  /* 5 */
    0x82,  /* 6 */
    0xF8,  /* 7 */
    0x80,  /* 8 */
    0x90   /* 9 */
};

/* ---- 扫描状态 ---- */
static unsigned char scan_flag;     /* 0=显示十位, 1=显示个位 */

/* ---- 主循环扫描函数: 每个系统节拍只切换一位 ---- */
void Display_Scan(void)
{
    unsigned char freq;

    freq = Freq_Get();

    /* 先关位选, 再切段码, 最后开目标位, 避免串位/鬼影 */
    DIG_TENS = 1;
    DIG_ONES = 1;

    if (scan_flag == 0) {
        if (freq >= 10) {
            SEG_PORT = seg_table[freq / 10];
        } else {
            SEG_PORT = 0xFF;
        }
        DIG_TENS = 0;
        scan_flag = 1;
    } else {
        SEG_PORT = seg_table[freq % 10];
        DIG_ONES = 0;
        scan_flag = 0;
    }
}

/* ---- Timer1 中断: 仅提供系统节拍 ---- */
void Timer1_ISR(void) interrupt 3 using 2
{
    static unsigned char div;

    if (++div >= DISP_TIMER_DIV) {
        div = 0;
        Tick_Notify();  /* 通知主循环 */
    }
}

/* ---- 初始化 ---- */
void Display_Init(void)
{
    scan_flag = 0;
    SEG_PORT = 0xFF;    /* 全部熄灭 (共阳, 高电平灭) */
    DIG_TENS = 1;       /* 十位未选中 */
    DIG_ONES = 1;       /* 个位未选中 */

    /* Timer1, Mode 2 (8位自动重装), 与 Timer0 Mode 1 共存 */
    TMOD = (TMOD & 0x0F) | 0x20;   /* 保留 Timer0 设置, Timer1 = Mode 2 */
    TH1 = 0;                        /* 重装值 0 → 周期 278us */
    TL1 = 0;

    ET1 = 1;    /* 允许 Timer1 中断, 仅提供系统节拍 */
}

/* ---- 启动扫描 (EA=1 后调用) ---- */
void Display_Start(void)
{
    TR1 = 1;
}
