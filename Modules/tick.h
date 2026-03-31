#ifndef TICK_H_
#define TICK_H_

/*
 * 系统节拍模块。
 *
 * 中断中调用 tick_notify() 置位节拍标志，
 * 前台通过 tick_get() 查询并使用 tick_clear() 清除。
 */

/* 中断中置位节拍标志 */
void tick_notify(void);

/* 读取当前节拍标志 */
unsigned char tick_get(void);

/* 清除节拍标志 */
void tick_clear(void);

#endif
