#include "delay.h"
#include <intrins.h>

/*
 * 延时模块实现
 * 晶振: 11.0592MHz, 机器周期 ≈ 1.085us
 */

void Delay_ms(unsigned int ms)
{
    unsigned int i, j;
    while (ms--) {
        i = 2;
        j = 199;
        do {
            while (--j);
        } while (--i);
    }
}

void Delay_us(unsigned int us)
{
    while (us--) {
        _nop_();    /* 每次循环约 3 个机器周期 (NOP+DJNZ) ≈ 3.3us */
    }
}
