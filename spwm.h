#ifndef SPWM_H_
#define SPWM_H_

#include "config.h"

/*
 * SPWM 波形发生器模块
 * 使用 Timer0 Mode1 (16位) 双相中断生成 SPWM
 */

void SPWM_Init(void);

#endif
