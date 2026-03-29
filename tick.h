#ifndef TICK_H_
#define TICK_H_

/*
 * 系统节拍模块
 *
 * Timer1 ISR 中置位, 主循环中查询并清除.
 * 独立于显示模块, 供任何需要定时轮询的模块使用.
 */

void Tick_Notify(void);     /* ISR 中调用: 置位节拍标志 */
unsigned char Tick_Get(void);  /* 主循环中查询 */
void Tick_Clear(void);         /* 主循环中清除 */

#endif
