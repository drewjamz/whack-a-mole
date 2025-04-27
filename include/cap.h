#ifndef CAP_H


#include <stdint.h>

#define CAP_ADR 0x29
#define I2C_SDA D4
#define I2C_SCL D5
#define IRQ D6

void config_cap();
void config_gpio_interrupt(void);
// extern volatile int cap_interrupt_flag;
// uint8_t cap_read_touch_status(void);
int hit_mole;

#endif