#include "cap.h"
#include "ee14lib.h"
#include "timer.h"

volatile int hit_mole = 0;

void config_gpio_interrupt(void) {
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG->EXTICR[0] &= ~(SYSCFG_EXTICR1_EXTI1);
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PB;
    EXTI->FTSR1 |= (1 << 1);
    EXTI->IMR1 |= (1 << 1);
    NVIC_SetPriority(EXTI1_IRQn, 2);
    NVIC_EnableIRQ(EXTI1_IRQn);

    printf("DEBUG: interrupts configured\n");
}

void config_cap() {

    delay_ms(5000);
    // printf("DEBUG: config cap\n");


    i2c_init(I2C1, I2C_SCL, I2C_SDA); //400KHz mode

    delay_ms(20);
    
    
    //Enable Sensor Input Enable Registers CS1_EN - CS5_EN
    unsigned char buf[] = {0x21, 0x1F};
    i2c_write(I2C1, CAP_ADR, buf, 2);
    
    //Enable Interrupts CS1_INT_EN - CS5_INT_EN
    unsigned char ie[] = { 0x27, 0x1F};
    i2c_write(I2C1, CAP_ADR, ie, 2);

    // Set to LEDs to Push-Pull
    unsigned char link[] = {0x71, 0xFF};
    i2c_write(I2C1, CAP_ADR, link, 2);

    //LEDs open drain
    unsigned char behavior[] = {0x72, 0x1F};
    i2c_write(I2C1, CAP_ADR, behavior, 2);

    //Link touch inputs to LEDs
    unsigned char repeat[] = {0x28, 0x00};
    i2c_write(I2C1, CAP_ADR, repeat, 2);

    
    //Does uh something i forget lowkey
    unsigned char buf2[] = {0x44, 0b01000001};
    i2c_write(I2C1, CAP_ADR, buf2, 2);

    //Sets sensitivity multiplier to 1x and leaves base same
    unsigned char buf3[] = {0x1F, 0b01001111};
    i2c_write(I2C1, CAP_ADR, buf3, 2);

    // 0x26 = Sensor Calibration Activate register
    unsigned char recalibrate_cmd[] = {0x26, 0x1F}; 
    i2c_write(I2C1, CAP_ADR, recalibrate_cmd, 2);

    delay_ms(600);

    gpio_config_mode(IRQ, INPUT);
    gpio_config_pullup(IRQ, PULL_UP);

    //gpio_config_mode(D11, OUTPUT); //test led pin

    // printf("DEBUG: touch capacitance sensor configured\n");
}

void EXTI1_IRQHandler(){

    // printf("DEBUG: IRQ Handler Entered\n");
    uint8_t reg = 0x03;
    uint8_t status = 0;

    //clears interrupt bit
    uint8_t cmd[] = { 0x00, 0x00 };  
    i2c_write(I2C1, CAP_ADR, cmd, 2);  

    unsigned char recalibrate_cmd[] = {0x26, 0x1F}; 
    i2c_write(I2C1, CAP_ADR, recalibrate_cmd, 2);

    i2c_write(I2C1, CAP_ADR, &reg, 1);  // point to status register
    i2c_read(I2C1, CAP_ADR, &status, 1); // read the status
    
    printf("%i\n", status);

    hit_mole = status;
    
    EXTI->PR1 = EXTI_PR1_PIF1;         // clear EXTI interrupt
    // printf("DEBUG: IRQ cleared\n");
}