#include "stm32l432xx.h"
#include "timer.h"

volatile uint32_t systick_count = 0;

void SysTick_initialize(void) {
    SysTick->CTRL = 0;
    SysTick->LOAD = 3999; // For 1 ms tick (assuming 4 MHz clock)
    SysTick->VAL = 0;

    // Lowest priority
    NVIC_SetPriority(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1);

    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

// Delay in milliseconds using systick_count
void delay_ms(uint32_t ms) {
    uint32_t target = systick_count + ms;
    while (systick_count < target);
}

// Handler called automatically by SysTick interrupt
void SysTick_Handler(void) {
    systick_count++;
}
