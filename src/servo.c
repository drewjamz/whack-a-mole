#include "servo.h"
#include "sertimer.h"

#define NUM_MOLES 5

static const EE14Lib_Pin mole_pins[NUM_MOLES] = {A0, A1, A2, D9, D10};  
static const TIM_TypeDef* mole_timers[NUM_MOLES] = {TIM2, TIM2, TIM2, TIM1, TIM1};

void  set_servo(TIM_TypeDef* const timer, const EE14Lib_Pin pin, int angle) {
    int duty = 51 + (angle * (102 - 51)) / 180;
    timer_config_channel_pwm(timer, pin, duty);
}

// Set all servos to 0 degrees
void servo_init() {
    timer_config_pwm(TIM2, 50);
    timer_config_pwm(TIM1, 50);

    for (int i = 0; i < NUM_MOLES; i++) {
        set_servo((TIM_TypeDef*)mole_timers[i], mole_pins[i], 90);
    }
}

void activate_mole(int mole) {
    set_servo((TIM_TypeDef*)mole_timers[mole], mole_pins[mole], 0);
}

void deactivate_mole(int mole) {
    set_servo((TIM_TypeDef*)mole_timers[mole], mole_pins[mole], 90);
}