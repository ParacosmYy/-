#include "spwm.h"
#include "freq.h"

/*
 * SPWM 核心模块
 *
 * 原理: 将一个正弦周期分为 SINE_POINTS 个采样点,
 *       每个采样点对应一个 PWM 占空比值 (查表法).
 *       Timer0 中断分为两相: LOW相 + HIGH相,
 *       通过改变两相的定时器重装载值来控制占空比.
 *
 * ISR 优化: 使用 q/r 分解法避免 32 位乘除法.
 *   sample_ticks[f] = tick_q[f] * PWM_LEVELS + tick_r[f]
 *   ticks = tick_q[f] * duty + (tick_r[f] * duty) >> 8
 *   PWM_LEVELS=256, 除法为右移 8 位, 所有中间运算均为 16 位.
 */

/* ---- 正弦查找表 (CODE 段, 存 ROM) ---- */
/* duty[i] = round( (sin(2*PI*i/100) + 1) / 2 * 255 ) */
const unsigned char code sine_table[SINE_POINTS] = {
   128, 136, 143, 151, 159, 167, 174, 182, 189, 196,
   202, 209, 215, 220, 226, 231, 235, 239, 243, 246,
   249, 251, 253, 254, 255, 255, 255, 254, 253, 251,
   249, 246, 243, 239, 235, 231, 226, 220, 215, 209,
   202, 196, 189, 182, 174, 167, 159, 151, 143, 136,
   128, 119, 112, 104,  96,  88,  81,  73,  66,  59,
    53,  46,  40,  35,  29,  24,  20,  16,  12,   9,
     6,   4,   2,   1,   0,   0,   0,   1,   2,   4,
     6,   9,  12,  16,  20,  24,  29,  35,  40,  46,
    53,  59,  66,  73,  81,  88,  96, 104, 112, 119
};

/*
 * 采样周期公式 (仅作文档参考, 数组已删除):
 * T_sample[f] = FOSC / 12 / (SINE_POINTS * freq)
 *             = 921600 / (100 * (f+1))  (f 为 0-based 索引)
 * 可从 tick_q / tick_r 还原: T_sample = tick_q[f] * 256 + tick_r[f]
 */

/* ---- q/r 分解表 (避免 ISR 中 32 位乘除法) ---- */
/*
 * sample_ticks[f] = tick_q[f] * 256 + tick_r[f]
 * ticks = tick_q[f] * duty + (tick_r[f] * duty) >> 8
 *
 * tick_q 和 tick_r 均 < 256, 与 duty (<256) 相乘后不超出 uint16,
 * ISR 中只需 16 位运算 + 右移 8 位.
 */
const unsigned char code tick_q[FREQ_MAX] = {
    36, 18, 12,  9,  7,  6,  5,  4,  4,  3,
     3,  3,  2,  2,  2,  2,  2,  2,  1,  1
};
const unsigned char code tick_r[FREQ_MAX] = {
     0,   0,   0,   0, 51,   0,  36, 128,   0, 153,
    69,   0, 196, 146, 102,  64,  30,   0, 229, 204
};

/* ---- 模块内部变量 ---- */
static unsigned char table_index;   /* 当前查表位置 (0~SINE_POINTS-1) */
static unsigned char phase;         /* 相位标志: 0=LOW相, 1=HIGH相 */
static unsigned char current_duty;  /* 当前占空比值 */

/* ---- Timer0 中断服务程序 ---- */
void Timer0_ISR(void) interrupt 1 using 1
{
    unsigned int ticks;
    unsigned int reload;
    unsigned int elapsed;
    unsigned char idx;
    unsigned char q;
    unsigned char r;
    unsigned char d;

    /* 不立即停定时器, 让它在计算期间继续计数, 便于补偿 */

    idx = Freq_Get() - 1;
    q = tick_q[idx];
    r = tick_r[idx];

    if (phase == 0) {
        /* ---- LOW 相: PWM 输出低电平 ---- */
        SPWM_PIN = 0;
        current_duty = sine_table[table_index];
        d = PWM_LEVELS - current_duty;
        phase = 1;
    } else {
        /* ---- HIGH 相: PWM 输出高电平 ---- */
        SPWM_PIN = 1;
        d = current_duty;

        /* 两相完成后才推进到下一个采样点 */
        table_index++;
        if (table_index >= SINE_POINTS) {
            table_index = 0;
        }

        phase = 0;
    }

    /* 16 位运算: q*d 最大 36*255=9180, r*d 最大 229*255=58395 */
    ticks = (unsigned int)q * d
          + (unsigned int)(((unsigned int)r * d) >> 8);

    if (ticks < MIN_PHASE_TICKS) {
        ticks = MIN_PHASE_TICKS;
    }

    /* 停定时器, 读取溢出后已计数的 tick 数, 补偿 ISR 耗时 */
    TR0 = 0;
    elapsed = ((unsigned int)TH0 << 8) | TL0;
    if (ticks > elapsed) {
        ticks -= elapsed;
    } else {
        ticks = MIN_PHASE_TICKS;
    }

    reload = (unsigned int)(0 - ticks);
    TH0 = (unsigned char)(reload >> 8);
    TL0 = (unsigned char)(reload & 0xFF);
    TR0 = 1;
}

/* ---- 初始化 ---- */
void SPWM_Init(void)
{
    table_index = 0;
    phase = 0;
    current_duty = sine_table[0];

    SPWM_PIN = 0;

    /* Timer0, Mode 1 (16位定时器) */
    TMOD &= 0xF0;
    TMOD |= 0x01;

    /* 初始装载: 从 1Hz 开始 */
    {
        unsigned int init_reload;
        init_reload = (unsigned int)(0 - ((unsigned int)tick_q[0] * PWM_LEVELS + tick_r[0]));
        TH0 = (unsigned char)(init_reload >> 8);
        TL0 = (unsigned char)(init_reload & 0xFF);
    }

    /* Timer0 高优先级, 可抢占 Timer1, 保证波形精度 */
    ET0 = 1;
    PT0 = 1;
}

/* ---- 启动输出 (EA=1 后调用) ---- */
void SPWM_Start(void)
{
    TR0 = 1;
}

