#include "stm32l432xx.h"
#include "ee14lib.h"
#include "timer.h"
#include "display.h"
#include "cap.h"
#include <stdio.h>

volatile uint8_t score = 0;
volatile uint8_t active_mole = 0;

typedef enum {
    STATE_MENU,
    STATE_GAME,
    STATE_GAME_OVER
} GameState;

GameState state = STATE_MENU;

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
    srand(systick_count);

    while (1) {
        switch (state) {
            case STATE_MENU:
                score = 0;
                display_score(score);
                if (hit_mole) {
                    state = STATE_GAME;
                    hit_mole = 0;
                }
                break;

            case STATE_GAME:
                hit_mole = 0;
                uint8_t n = (rand() % 5) - 1;
                active_mole = 1 << n;  // Set active mole in 2^n form

                printf("Active mole: 0x%02X\n", active_mole);

                //CANNOT USE DELAY HERE: display score must always run.
                uint32_t start = systick_count;
                while (systick_count - start < 5000) {
                    display_score(score);
                }

                printf("Hit mole: 0x%02X\n", hit_mole);
                printf("\n");

                if (hit_mole & active_mole) {
                    mole_hit();
                } else {
                    state = STATE_GAME_OVER;
                }

                display_score(score);  // Show current score on 7-seg display

                // Short delay before next mole
                start = systick_count;
                while (systick_count - start < 300) {
                    display_score(score);
                }

                break;

            case STATE_GAME_OVER: 
                start = systick_count;
                while (systick_count - start < 10000) {
                    display_score(score);  // show final score
                }
                state = STATE_MENU;
                break;
        }
    }
}

// Game control functions
void mole_hit(void) {
    if (score <= 99) score++;
    else score = 0;
}