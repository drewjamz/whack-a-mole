#include "stm32l432xx.h"
#include "ee14lib.h"
#include "timer.h"
#include "display.h"
#include <stdio.h>

volatile uint8_t score = 0;

//allows us to use printf
int _write(int file, char *data, int len) {
    serial_write(USART2, data, len);
    return len;
}

int main() {
    SysTick_initialize();
    display_init();

    while (1) {
        //score logic
        //display_score(score);
    }
}

// Game control functions
void mole_hit(void) {
    if (score < 99) score++;
}

void mole_miss(void) {
    score = 0;
}
