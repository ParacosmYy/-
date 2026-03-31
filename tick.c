#include "tick.h"

/*
 * 系统节拍模块实现。
 *
 * 使用单字节标志在中断与前台之间传递节拍事件。
 */

static volatile unsigned char s_tick = 0U;

void Tick_Notify(void)
{
    s_tick = 1U;
}

unsigned char Tick_Get(void)
{
    return s_tick;
}

void Tick_Clear(void)
{
    s_tick = 0U;
}
