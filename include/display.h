#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

// Public function declarations
void display_init(void);
void display_score(uint8_t score);
void mole_hit(void);
void mole_miss(void);

#endif