#ifndef FREQ_H_
#define FREQ_H_

#include "config.h"

/*
 * 频率管理模块。
 *
 * 负责维护当前输出频率，并提供带边界保护的读写接口。
 * 中断和前台代码统一通过本模块访问频率值。
 */

/* 获取当前频率值 */
unsigned char freq_get(void);

/* 频率加 1，达到上限后保持不变 */
void freq_increase(void);

/* 频率减 1，达到下限后保持不变 */
void freq_decrease(void);

#endif
