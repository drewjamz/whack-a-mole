#include "stm32l432xx.h"
#include "ee14lib.h"
#include "timer.h"
#include "display.h"
#include "cap.h"
#include <stdio.h>

volatile uint8_t score = 0;
volatile uint8_t active_mole = 0;

//allows us to use printf
int _write(int file, char *data, int len) {
    serial_write(USART2, data, len);
    return len;
}

int main() {
    host_serial_init();
    SysTick_initialize();
    display_init();
    config_cap();
    config_gpio_interrupt();

    while (1) {
        display_score(score);
        
        if (hit_mole == active_mole) mole_hit();
        else mole_miss();
    }
}

// Game control functions
void mole_hit(void) {
    if (score < 99) score++;
}

void mole_miss(void) {
    score = 0;
}
