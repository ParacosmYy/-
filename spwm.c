#include "spwm.h"

/*
 * SPWM 核心模块
 *
 * 原理: 将一个正弦周期分为 SINE_POINTS 个采样点,
 *       每个采样点对应一个 PWM 占空比值 (查表法).
 *       Timer0 中断分为两相: LOW相 + HIGH相,
 *       通过改变两相的定时器重装载值来控制占空比.
 *
 * 频率调节: 修改 g_frequency 后, ISR 自动查表获取新的采样周期.
 */

/* ---- 正弦查找表 (CODE 段, 存 ROM) ---- */
/* duty[i] = round( (sin(2*PI*i/100) + 1) / 2 * 99 ) */
const unsigned char code sine_table[SINE_POINTS] = {
    50, 53, 56, 59, 62, 65, 68, 71, 73, 76,
    79, 81, 83, 86, 88, 90, 91, 93, 94, 96,
    97, 97, 98, 99, 99, 99, 99, 99, 98, 97,
    97, 96, 94, 93, 91, 90, 88, 86, 83, 81,
    79, 76, 73, 71, 68, 65, 62, 59, 56, 53,
    50, 46, 43, 40, 37, 34, 31, 28, 26, 23,
    20, 18, 16, 13, 11,  9,  8,  6,  5,  3,
     2,  2,  1,  0,  0,  0,  0,  0,  1,  2,
     2,  3,  5,  6,  8,  9, 11, 13, 16, 18,
    20, 23, 26, 28, 31, 34, 37, 40, 43, 46
};

/* ---- 各频率对应的采样周期 (机器周期数) ---- */
/*
 * T_sample = FOSC / 12 / (SINE_POINTS * freq)
 * 11.0592MHz / 12 = 921600 Hz (1 machine cycle = 1.085us)
 *
 * sample_ticks[f] = 921600 / (100 * (f+1))
 */
const unsigned int code sample_ticks[FREQ_MAX] = {
     9216,  /*  1Hz */
     4608,  /*  2Hz */
     3072,  /*  3Hz */
     2304,  /*  4Hz */
     1843,  /*  5Hz */
     1536,  /*  6Hz */
     1317,  /*  7Hz */
     1152,  /*  8Hz */
     1024,  /*  9Hz */
      922,  /* 10Hz */
      838,  /* 11Hz */
      768,  /* 12Hz */
      709,  /* 13Hz */
      658,  /* 14Hz */
      614,  /* 15Hz */
      576,  /* 16Hz */
      542,  /* 17Hz */
      512,  /* 18Hz */
      485,  /* 19Hz */
      461   /* 20Hz */
};

/* ---- 模块内部变量 ---- */
static unsigned char table_index;   /* 当前查表位置 (0~SINE_POINTS-1) */
static unsigned char phase;         /* 相位标志: 0=LOW相, 1=HIGH相 */
static unsigned char current_duty;  /* 当前占空比值 */

/* ---- Timer0 中断服务程序 ---- */
void Timer0_ISR(void) interrupt 1
{
    unsigned int ticks;
    unsigned int reload;

    TR0 = 0;    /* 停止定时器 */

    if (phase == 0) {
        /* ---- LOW 相: PWM 输出低电平 ---- */
        SPWM_PIN = 0;

        current_duty = sine_table[table_index];

        /* 低电平持续时间 = 采样周期 × (100-duty) / 100 */
        /* 使用 unsigned long 防止乘法溢出 (8051 int 为16位) */
        ticks = (unsigned long)sample_ticks[g_frequency - 1]
                * (PWM_LEVELS - current_duty) / PWM_LEVELS;

        if (ticks < MIN_PHASE_TICKS) {
            ticks = MIN_PHASE_TICKS;
        }

        reload = 65536 - ticks;
        TH0 = (unsigned char)(reload >> 8);
        TL0 = (unsigned char)(reload & 0xFF);

        phase = 1;
    } else {
        /* ---- HIGH 相: PWM 输出高电平 ---- */
        SPWM_PIN = 1;

        /* 使用当前采样点的 duty 计算 HIGH 时间 (与 LOW 相同一个点) */
        ticks = (unsigned long)sample_ticks[g_frequency - 1]
                * current_duty / PWM_LEVELS;

        if (ticks < MIN_PHASE_TICKS) {
            ticks = MIN_PHASE_TICKS;
        }

        reload = 65536 - ticks;
        TH0 = (unsigned char)(reload >> 8);
        TL0 = (unsigned char)(reload & 0xFF);

        /* 两相完成后才推进到下一个采样点 */
        table_index++;
        if (table_index >= SINE_POINTS) {
            table_index = 0;
        }

        phase = 0;
    }

    TR0 = 1;    /* 重启定时器 */
}

/* ---- 初始化 ---- */
void SPWM_Init(void)
{
    table_index = 0;
    phase = 0;
    current_duty = sine_table[0];

    SPWM_PIN = 0;

    /* Timer0, Mode 1 (16位定时器) */
    TMOD &= 0xF0;      /* 清除 Timer0 低4位 */
    TMOD |= 0x01;      /* Mode 1: 16位 */

    /* 初始装载: 从 1Hz 开始 */
    {
        unsigned int init_reload;
        init_reload = 65536 - sample_ticks[0];
        TH0 = (unsigned char)(init_reload >> 8);
        TL0 = (unsigned char)(init_reload & 0xFF);
    }

    /* 开中断 */
    ET0 = 1;
    EA = 1;

    /* 启动定时器 */
    TR0 = 1;
}

/* ---- 更新频率 (按键调用) ---- */
void SPWM_UpdateFreq(void)
{
    /*
     * ISR 在下次中断时自动使用新的 g_frequency 查表,
     * 无需额外操作. 此函数留作接口扩展.
     */
}
