#include "ee14lib.h"

// Mapping of Nucleo pin number to GPIO port
static GPIO_TypeDef * g_GPIO_port[D13+1] = {
  GPIOA,GPIOA,GPIOA,GPIOA,  // A0=PA0,A1=PA1,A2=PA3,A3=PA4
  GPIOA,GPIOA,GPIOA,GPIOA,  // A4=PA5,A5=PA6,A6=PA7,A7=PA2
  GPIOA,GPIOA,GPIOA,GPIOB,  // D0=PA10,D1=PA9,D2=PA12,D3=PB0
  GPIOB,GPIOB,GPIOB,GPIOC,  // D4=PB7,D5=PB6,D6=PB1,D7=PC14
  GPIOC,GPIOA,GPIOA,GPIOB,  // D8=PC15,D9=PA8,D10=PA11,D11=PB5
  GPIOB,GPIOB               // D12=PB4,D13=PB3.
};

// Mapping of Nucleo pin number to GPIO pin
// Using this plust g_GPIO_port[] above, we can translate a Nucleo pin name into
// the chip's actual GPIO port and pin number.
static uint8_t g_GPIO_pin[D13+1] = {
  0,1,3,4,    // A0=PA0,A1=PA1,A2=PA3,A3=PA4
  5,6,7,2,    // A4=PA5,A5=PA6,A6=PA7,A7=PA2
  10,9,12,0,  // D0=PA10,D1=PA9,D2=PA12,D3=PB0
  7,6,1,14,   // D4=PB7,D5=PB6,D6=PB1,D7=PC14
  15,8,11,5,  // D8=PC15,D9=PA8,D10=PA11,D11=PB5
  4,3         // D12=PB4,D13=PB3.
};

// Enables a GPIO port (A, B, C, or H) by setting the appropriate bit in the RCC
// clock enable register.
//   gpio: Pointer to GPIO port to enable, one of GPIOA, GPIOB, GPIOC, GPIOH
static void gpio_enable_port (GPIO_TypeDef *gpio) {
    unsigned long field;
    if (gpio==GPIOA)      field=RCC_AHB2ENR_GPIOAEN;
    else if (gpio==GPIOB) field=RCC_AHB2ENR_GPIOBEN;
    else if (gpio==GPIOC) field=RCC_AHB2ENR_GPIOCEN;
    else           field=RCC_AHB2ENR_GPIOHEN;
    RCC->AHB2ENR |= field; // Turn on the GPIO clock
}

// Configure a GPIO pin for one of its "alternate functions".  See Tables 15 and
// 16 of the STM32L432KC datasheet for a complete listing of the alternate
// functions for each pin.
//   pin: A Nucleo pin ID (D2, A4, etc.)
//   function: an integer 0-15 to select the alternate function
// Always returns EE14Lib_Err_OK; in the future this may return errors for
// invalid configurations.
EE14Lib_Err gpio_config_alternate_function(EE14Lib_Pin pin, unsigned int function)
{
    GPIO_TypeDef* port = g_GPIO_port[pin];
    uint8_t pin_offset = g_GPIO_pin[pin];

    // Enable the GPIO port in case it hasn't been already
    gpio_enable_port(port);

    // Set the GPIO pin mode to "alternate function" (0b10)
    port->MODER &= ~(0b11 << pin_offset*2); // Clear both bits
    port->MODER |=  (0b10 << pin_offset*2); // 0b10 = alternate function mode

    // Set the AFR register
    unsigned int afr_offset = pin_offset * 4; // 4 bits per pin -> value from 0 to 60

    port->AFR[afr_offset >> 5] &= ~(0xF << (0x1F & afr_offset));
    port->AFR[afr_offset >> 5] |=  (function << (0x1F & afr_offset));

    return EE14Lib_Err_OK;
}
