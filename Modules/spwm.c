#include "spwm.h"
#include "freq.h"

/*
 * SPWM output module.
 *
 * Timer0 uses a sine lookup table and dual-phase timing to generate SPWM.
 * Frequency timing parameters are precomputed as q/r tables for ISR speed.
 */

/* Sine lookup table, range 0..255. */
const unsigned char code spwm_sine_table[SINE_POINTS] = {
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
 * Sample period:
 * T_sample = FOSC / 12 / (SINE_POINTS * freq)
 *
 * The q/r split avoids 32-bit division inside the ISR.
 */
const unsigned char code spwm_tick_q[FREQ_MAX] = {
    36, 18, 12,  9,  7,  6,  5,  4,  4,  3,
     3,  3,  2,  2,  2,  2,  2,  2,  1,  1
};

const unsigned char code spwm_tick_r[FREQ_MAX] = {
     0,   0,   0,   0, 51,   0,  36, 128,   0, 153,
    69,   0, 196, 146, 102,  64,  30,   0, 229, 204
};

static unsigned char s_spwm_table_index;
static unsigned char s_spwm_phase;
static unsigned char s_spwm_current_duty;
static unsigned char s_cached_freq_idx;

void Timer0_ISR(void) interrupt 1 using 1
{
    unsigned int ticks;
    unsigned int reload;
    unsigned int elapsed;
    unsigned char q;
    unsigned char r;
    unsigned char duty;

    if (s_spwm_phase == 0U) {
        s_cached_freq_idx = freq_get() - 1U;
        q = spwm_tick_q[s_cached_freq_idx];
        r = spwm_tick_r[s_cached_freq_idx];
        SPWM_PIN = 0;
        s_spwm_current_duty = spwm_sine_table[s_spwm_table_index];
        duty = PWM_LEVELS - s_spwm_current_duty;
        s_spwm_phase = 1U;
    } else {
        q = spwm_tick_q[s_cached_freq_idx];
        r = spwm_tick_r[s_cached_freq_idx];
        SPWM_PIN = 1;
        duty = s_spwm_current_duty;

        s_spwm_table_index++;
        if (s_spwm_table_index >= SINE_POINTS) {
            s_spwm_table_index = 0U;
        }

        s_spwm_phase = 0U;
    }

    ticks = (unsigned int)q * duty
          + (unsigned int)(((unsigned int)r * duty) >> 8);

    if (ticks < MIN_PHASE_TICKS) {
        ticks = MIN_PHASE_TICKS;
    }

    TR0 = 0;
    elapsed = ((unsigned int)TH0 << 8) | TL0;
    if (ticks > elapsed) {
        ticks -= elapsed;
    } else {
        ticks = MIN_PHASE_TICKS;
    }

    reload = (unsigned int)(0U - ticks);
    TH0 = (unsigned char)(reload >> 8);
    TL0 = (unsigned char)(reload & 0xFF);
    TR0 = 1;
}

void spwm_init(void)
{
    unsigned int init_reload;

    s_spwm_table_index = 0U;
    s_spwm_phase = 0U;
    s_spwm_current_duty = spwm_sine_table[0];
    s_cached_freq_idx = FREQ_MIN - 1U;

    SPWM_PIN = 0;

    TMOD &= 0xF0;
    TMOD |= 0x01;

    init_reload = (unsigned int)(0U - ((unsigned int)spwm_tick_q[0] * PWM_LEVELS + spwm_tick_r[0]));
    TH0 = (unsigned char)(init_reload >> 8);
    TL0 = (unsigned char)(init_reload & 0xFF);

    ET0 = 1;
    PT0 = 1;
}

void spwm_start(void)
{
    TR0 = 1;
}
