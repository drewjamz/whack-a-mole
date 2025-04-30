#include "ee14lib.h"

int main() {
    //Check basic servo values
    for (int i = 0; i <= 180; i += 45) {
        set_servo(TIM2, A0, i);
    }
    
    return 0;
}

/*  Set servo to an angle between 0 and 180, specifying GPIO output pin and
    pwm timer.
*/
void  set_servo(TIM_TypeDef* const timer, const EE14Lib_Pin pin, int angle) {
    timer_config_pwm(timer, 50);

    int duty = (angle/180) * 100;
    timer_config_channel_pwm(timer, pin, duty);
}