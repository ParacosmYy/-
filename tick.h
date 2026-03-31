#ifndef TICK_H_
#define TICK_H_

/*
 * 系统节拍模块。
 *
 * 中断中调用 Tick_Notify() 置位节拍标志，
 * 前台通过 Tick_Get() 查询并使用 Tick_Clear() 清除。
 */

/* 中断中置位节拍标志 */
void Tick_Notify(void);

/* 读取当前节拍标志 */
unsigned char Tick_Get(void);

/* 清除节拍标志 */
void Tick_Clear(void);

#endif
