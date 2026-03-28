#include "display.h"

/*
 * 数码管显示模块 — Timer1 中断驱动
 *
 * Timer1 Mode 2 (8位自动重装), 重装值 0 → 溢出周期 256 × 1.085us ≈ 278us.
 * 软件分频 DISP_TIMER_DIV = 7 → 显示扫描周期 ≈ 1.94ms ≈ 2ms.
 * 每次扫描切换一位数码管, 同时置 g_tick 供主循环使用.
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

/* ---- 2ms 系统节拍 ---- */
volatile unsigned char g_tick = 0;

/* ---- Timer1 中断: 显示扫描 + 系统节拍 ---- */
void Timer1_ISR(void) interrupt 3 using 2
{
    static unsigned char div;

    if (++div >= DISP_TIMER_DIV) {
        div = 0;

        /* 先熄灭, 消鬼影 */
        DIG_TENS = 1;
        DIG_ONES = 1;

        if (scan_flag == 0) {
            /* ---- 显示十位 (频率 < 10 时消隐) ---- */
            if (g_frequency >= 10) {
                SEG_PORT = seg_table[g_frequency / 10];
            } else {
                SEG_PORT = 0xFF;    /* 消隐: 全部段灭 */
            }
            DIG_TENS = 0;
            scan_flag = 1;
        } else {
            /* ---- 显示个位 ---- */
            SEG_PORT = seg_table[g_frequency % 10];
            DIG_ONES = 0;
            scan_flag = 0;
        }

        g_tick = 1;    /* 通知主循环 */
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

    ET1 = 1;    /* 允许 Timer1 中断 */
    TR1 = 1;    /* 启动 Timer1 */
}
