#include "tick.h"

/*
 * 系统节拍 — 单标志实现
 *
 * ISR 写 (Tick_Notify), 主循环读 (Tick_Get) 并清 (Tick_Clear).
 * 单字节读写在 8051 上天然原子, 无需关中断.
 */

static volatile unsigned char s_tick = 0;

void Tick_Notify(void)        { s_tick = 1; }
unsigned char Tick_Get(void)  { return s_tick; }
void Tick_Clear(void)         { s_tick = 0; }
