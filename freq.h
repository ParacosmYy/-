#ifndef FREQ_H_
#define FREQ_H_

#include "config.h"

/*
 * 频率管理模块
 *
 * 封装频率状态, 提供带边界保护的读写接口.
 * 所有写操作关中断执行, 保证 ISR 读到一致值.
 */

unsigned char Freq_Get(void);
void Freq_Inc(void);
void Freq_Dec(void);

#endif
