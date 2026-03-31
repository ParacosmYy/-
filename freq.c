#include "freq.h"

/*
 * 频率管理模块实现。
 *
 * 当前频率由模块私有变量保存。
 * 写操作在临界区内完成，避免前后台并发访问时出现不一致。
 */

static volatile unsigned char s_frequency = FREQ_MIN;

unsigned char Freq_Get(void)
{
    return s_frequency;
}

void Freq_Inc(void)
{
    unsigned char ea;

    if (s_frequency >= FREQ_MAX) {
        return;
    }

    ea = EA;
    EA = 0;
    s_frequency++;
    EA = ea;
}

void Freq_Dec(void)
{
    unsigned char ea;

    if (s_frequency <= FREQ_MIN) {
        return;
    }

    ea = EA;
    EA = 0;
    s_frequency--;
    EA = ea;
}
