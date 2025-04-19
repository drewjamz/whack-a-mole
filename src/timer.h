#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void delay_ms(uint32_t ms);
void SysTick_initialize(void);

extern volatile uint32_t systick_count;

#endif
