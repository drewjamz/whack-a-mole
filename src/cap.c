#include "cap.h"
#include "ee14lib.h"

#define CAP_ADR 0x29
#define I2C_SDA D4
#define I2C_SCL D5

void config_cap() {
    i2c_init(I2C1, I2C_SCL, I2C_SDA); //400KHz mode
    
    //Enable Sensor Input Enable Registers CS1_EN - CS5_EN
    unsigned char buf[] = {0x21, 0x1F};
    if (!i2c_write(I2C1, CAP_ADR, &buf, 2)) {
        printf("Error: Did not receive ACK\n");
        return 0;
    }

    //set up an interrupt for checking touch status at 0x03
    //clear interrupts, write 0x00 to 0x00
    //0x72 
    // read_touch_status();


}

void EXTI1_IRQHandler(){
    
}

// unsigned char read_touch_status() {
//     unsigned char reg[] = {0x03};
//     unsigned char data = 0;

//     // First write the register address we want to read from
//     if (!i2c_write(I2C1, CAP_ADR, &reg, 1)) {
//         // handle error
//         return 0;
//     }

//     // Then read 1 byte from that register
//     if (!i2c_read(I2C1, CAP_ADR, &data, 1)) {
//         // handle error
//         return 0;
//     }

//     return data;
// }
