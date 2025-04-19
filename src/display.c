#include "display.h"
#include "timer.h"
#include "ee14lib.h"

// GPIO pins
#define SR_DATA     D2
#define SR_CLOCK    D3
#define SR_LATCH    D4

#define DIGIT_1     D9
#define DIGIT_2     D10

static const uint8_t segment_map[10] = {
    0b1111110,  // 0
    0b0110000,  // 1
    0b1101101,  // 2
    0b1111001,  // 3
    0b0110011,  // 4
    0b1011011,  // 5
    0b1011111,  // 6
    0b1110000,  // 7
    0b1111111,  // 8
    0b1111011   // 9
};

void display_init(void) {
    gpio_config_mode(SR_DATA, OUTPUT);
    gpio_config_mode(SR_CLOCK, OUTPUT);
    gpio_config_mode(SR_LATCH, OUTPUT);

    gpio_config_mode(DIGIT_1, OUTPUT);
    gpio_config_mode(DIGIT_2, OUTPUT);

    gpio_write(DIGIT_1, 0);
    gpio_write(DIGIT_2, 0);
}

static void shift_out(uint8_t val) {
    for (int i = 7; i >= 0; i--) {
        gpio_write(SR_CLOCK, 0);
        gpio_write(SR_DATA, (val >> i) & 1);
        gpio_write(SR_CLOCK, 1);
    }
}

static void display_digit(uint8_t digit) {
    gpio_write(SR_LATCH, 0);
    shift_out(segment_map[digit % 10]);
    gpio_write(SR_LATCH, 1);
}

// Call this repeatedly in main to refresh both digits
void display_score(uint8_t score_val) {
    uint8_t tens = score_val / 10;
    uint8_t ones = score_val % 10;

    // Tens digit
    display_digit(tens);
    gpio_write(DIGIT_1, 1);
    delay_ms(5);
    gpio_write(DIGIT_1, 0);

    // Ones digit
    display_digit(ones);
    gpio_write(DIGIT_2, 1);
    delay_ms(5);
    gpio_write(DIGIT_2, 0);
}

