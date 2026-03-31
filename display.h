#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "config.h"

/*
 * Foreground display driver for a dual common-anode 7-segment module.
 *
 * Display refresh is executed only from the main while-loop.
 * The timer is not used to drive multiplexing.
 */

void Display_Init(void);
void Display_SetValue(unsigned char value);
void Display_Task(void);

#endif
