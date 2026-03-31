#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "config.h"

/*
 * Dual 7-segment display driver.
 *
 * Timer1 only provides the system tick.
 * The foreground calls Display_Task() periodically to multiplex one digit
 * per invocation. The module owns the display cache so upper layers only
 * provide the value to be shown.
 */

#define DISP_TIMER_DIV  7U

void Display_Init(void);
void Display_Start(void);
void Display_SetValue(unsigned char value);
void Display_Task(void);

#endif
