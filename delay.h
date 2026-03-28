#ifndef __DELAY_H__
#define __DELAY_H__

/*
 * 延时模块 — 基于 11.0592MHz 晶振
 * 机器周期 = 12 / 11.0592MHz ≈ 1.085us
 */

void Delay_ms(unsigned int ms);
void Delay_us(unsigned int us);

#endif
