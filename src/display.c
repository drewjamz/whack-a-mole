#include "display.h"
#include "ee14lib.h"
#include "timer.h"
#include "stm32l432xx.h"

// GPIO pins
#define SR_DATA     A1
#define SR_CLOCK    A2
#define SR_CLEAR    A0

#define DIGIT_1     D9
#define DIGIT_2     D10

static const uint8_t segment_map[10] = {
    0b11111100,  // 0
    0b01100000,  // 1
    0b11011010,  // 2
    0b11110010,  // 3
    0b01100110,  // 4
    0b10110110,  // 5
    0b10111110,  // 6
    0b11100000,  // 7
    0b11111110,  // 8
    0b11110110   // 9
};

void display_init(void) {
    gpio_config_mode(SR_DATA, OUTPUT);
    gpio_config_mode(SR_CLOCK, OUTPUT);
    // gpio_config_mode(SR_LATCH, OUTPUT);

    gpio_config_mode(DIGIT_1, OUTPUT);
    gpio_config_mode(DIGIT_2, OUTPUT);

    gpio_write(DIGIT_1, 0);
    gpio_write(DIGIT_2, 0);

    gpio_config_mode(SR_CLEAR, OUTPUT);
    gpio_write(SR_CLEAR, 1);

}

static void shift_out(uint8_t val) {
    for (int i = 0; i < 8; i++) {
        gpio_write(SR_CLOCK, 0);
        gpio_write(SR_DATA, (val >> i) & 1);  // ensures output is 0 or 1
        gpio_write(SR_CLOCK, 1);
    }
}

static void display_digit(uint8_t digit) {
    // gpio_write(SR_LATCH, 0);
    shift_out(~segment_map[digit % 10]);
    // gpio_write(SR_LATCH, 1);
}

// Call this repeatedly in main to refresh both digits
void display_score(uint8_t score_val) {
    uint8_t tens = score_val / 10;
    uint8_t ones = score_val % 10;

    // Tens digit
    gpio_write(DIGIT_2, 0);
    display_digit(tens);
    gpio_write(DIGIT_1, 1);
    delay_ms(10);

    // Ones digit
    gpio_write(DIGIT_1, 0);
    display_digit(ones);
    gpio_write(DIGIT_2, 1);
    delay_ms(10);

}


