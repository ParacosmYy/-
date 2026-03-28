#include "config.h"
#include "display.h"
#include "delay.h"

/*
 * 数码管显示模块
 *
 * 采用两位一体共阳七段数码管, 动态扫描显示.
 * 共阳: 段选低电平点亮, 位选低电平选中.
 */

/* ---- 共阳七段码查找表 (0~9) ---- */
/* 段选编码: [dp g f e d c b a], 低电平有效 */
const unsigned char code seg_table[10] = {
    0xC0,  /* 0: a b c d e f     */
    0xF9,  /* 1: b c             */
    0xA4,  /* 2: a b d e g       */
    0xB0,  /* 3: a b c d g       */
    0x99,  /* 4: b c f g         */
    0x92,  /* 5: a c d f g       */
    0x82,  /* 6: a c d e f g     */
    0xF8,  /* 7: a b c           */
    0x80,  /* 8: a b c d e f g   */
    0x90   /* 9: a b c d f g     */
};

/* ---- 扫描状态 ---- */
static unsigned char scan_flag;     /* 0=显示十位, 1=显示个位 */

/* ---- 初始化 ---- */
void Display_Init(void)
{
    scan_flag = 0;
    SEG_PORT = 0xFF;    /* 全部熄灭 (共阳, 高电平灭) */
    DIG_TENS = 1;       /* 十位未选中 */
    DIG_ONES = 1;       /* 个位未选中 */
}

/* ---- 动态扫描刷新 (主循环中调用) ---- */
void Display_Refresh(void)
{
    /* 先熄灭, 消鬼影 */
    DIG_TENS = 1;
    DIG_ONES = 1;

    if (scan_flag == 0) {
        /* ---- 显示十位 ---- */
        SEG_PORT = seg_table[g_frequency / 10];
        DIG_TENS = 0;       /* 选中十位 */
        scan_flag = 1;
    } else {
        /* ---- 显示个位 ---- */
        SEG_PORT = seg_table[g_frequency % 10];
        DIG_ONES = 0;       /* 选中个位 */
        scan_flag = 0;
    }

    /* 延时保持显示 */
    Delay_ms(DISP_SCAN_MS);
}
