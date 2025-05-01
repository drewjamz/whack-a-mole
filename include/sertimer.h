#ifndef SERVOTIMER_H
#define SERVOTIMER_H

#include "stm32l432xx.h"
#include "ee14lib.h"

// Configure timer to generate PWM at the given frequency (Hz)
EE14Lib_Err timer_config_pwm(TIM_TypeDef* const timer, const unsigned int freq_hz);

// Configure a specific timer channel to output a PWM signal with a duty cycle [0–1023]
EE14Lib_Err timer_config_channel_pwm(TIM_TypeDef* const timer, const EE14Lib_Pin pin, const unsigned int duty);

#endif // SERVOTIMER_H
