#include "freq.h"

/*
 * 频率管理模块
 *
 * 单一状态源: s_frequency, 仅通过本模块接口访问.
 * 写操作含边界判断 + 关中断保护, 保证并发安全.
 */

static volatile unsigned char s_frequency = FREQ_MIN;

/* ---- 读取 (8051 单字节读天然原子, 无需关中断) ---- */
unsigned char Freq_Get(void)
{
    return s_frequency;
}

/* ---- 递增 (已达上限时直接返回, 避免不必要的关中断) ---- */
void Freq_Inc(void)
{
    unsigned char ea;
    if (s_frequency >= FREQ_MAX) return;
    ea = EA;
    EA = 0;
    s_frequency++;
    EA = ea;
}

/* ---- 递减 (已达下限时直接返回, 避免不必要的关中断) ---- */
void Freq_Dec(void)
{
    unsigned char ea;
    if (s_frequency <= FREQ_MIN) return;
    ea = EA;
    EA = 0;
    s_frequency--;
    EA = ea;
}
