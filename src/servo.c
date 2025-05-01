#include "servo.h"
#include "sertimer.h"

#define NUM_MOLES 5

static const EE14Lib_Pin mole_pins[NUM_MOLES] = {A0, A1, A2, D3, D13};  
static const TIM_TypeDef* mole_timers[NUM_MOLES] = {TIM2, TIM2, TIM2, TIM2, TIM2};

void  set_servo(TIM_TypeDef* const timer, const EE14Lib_Pin pin, int angle) {
    timer_config_pwm(timer, 50);

    int duty = 51 + (angle * (102 - 51)) / 180;
    timer_config_channel_pwm(timer, pin, duty);
}

// Set all servos to 0 degrees
void servo_init() {
    for (int i = 0; i < NUM_MOLES; i++) {
        set_servo((TIM_TypeDef*)mole_timers[i], mole_pins[i], 0);
    }
}

void activate_mole(int mole) {
    set_servo((TIM_TypeDef*)mole_timers[mole - 1], mole_pins[mole - 1], 90);
}

void deactivate_mole(int mole) {
    set_servo((TIM_TypeDef*)mole_timers[mole - 1], mole_pins[mole - 1], 90);
}