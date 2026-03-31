#include "freq.h"

/*
 * 频率管理模块实现。
 *
 * 当前频率由模块私有变量保存。
 * 写操作在临界区内完成，避免前后台并发访问时出现不一致。
 */

static volatile unsigned char s_frequency = FREQ_MIN;

unsigned char freq_get(void)
{
    return s_frequency;
}

void freq_increase(void)
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

void freq_decrease(void)
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
