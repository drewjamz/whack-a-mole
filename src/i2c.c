#include "ee14lib.h"

void i2c_init(I2C_TypeDef* i2c, EE14Lib_Pin scl, EE14Lib_Pin sda)
{
    // Configure GPIO pins for I2C: open-drain and internal pullup
    gpio_config_otype(scl, OPEN_DRAIN);
    gpio_config_otype(sda, OPEN_DRAIN);
    // TODO: enable "fast mode plus" instead of ospeed
    //SYSCFG->CFGR1 |= SYSCFG_CFGR1_I2C1_FMP;
    //gpio_config_ospeed(scl, V_HI_SPD);
    //gpio_config_ospeed(sda, V_HI_SPD);
    gpio_config_pullup(scl, PULL_UP);
    gpio_config_pullup(sda, PULL_UP);
    gpio_config_alternate_function(scl, 4); // AF4 is for I2C
    gpio_config_alternate_function(sda, 4);

    // Enable clock for the appropriate I2C device
    if(i2c == I2C1){
        RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;
        RCC->CCIPR &= ~RCC_CCIPR_I2C1SEL;
        RCC->CCIPR |= 0b01 << RCC_CCIPR_I2C1SEL_Pos; // Use SYSCLK for I2C

        // Reset the I2C module now that its clock is configured
        RCC->APB1RSTR1 |= RCC_APB1RSTR1_I2C1RST;
        RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_I2C1RST;
    }
    else if(i2c == I2C3){
        RCC->APB1ENR1 |= RCC_APB1ENR1_I2C3EN;
        RCC->CCIPR &= ~RCC_CCIPR_I2C3SEL;
        RCC->CCIPR |= 0b01 << RCC_CCIPR_I2C3SEL_Pos; // Use SYSCLK for I2C

        // Reset the I2C module now that its clock is configured
        RCC->APB1RSTR1 |= RCC_APB1RSTR1_I2C3RST;
        RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_I2C3RST;
    }
    else {
        return EE14Lib_ERR_INVALID_CONFIG; // Invalid I2C device
    }

    // Configuration registers 
    // Everything is zero coming out of reset and default settings are ok
    // TODO: better error handling with error interrupt?
    //i2c->CR1 |= I2C_CR1_ERRIE;

    // Everything in CR2 is specific to a single transaction, so don't touch it here

    // Now configure the timing
    // This gives roughly 400kHz timing
    i2c->TIMINGR |= 0 << I2C_TIMINGR_PRESC_Pos |
                    0 << I2C_TIMINGR_SCLDEL_Pos | // Stretch low time to get enough setup time
                    1 << I2C_TIMINGR_SCLH_Pos |
                    1 << I2C_TIMINGR_SCLL_Pos;

    // Leave own address disabled; we're only operating as the controller

    i2c->CR1 |= I2C_CR1_PE; // Finally, enable the I2C peripheral
}


bool i2c_write(I2C_TypeDef* i2c, unsigned char device_address, unsigned char* data, unsigned char len)
{
    // Wait until the I2C peripheral is not busy
    // If there were other hosts on the line, we'd need to check that both SDA + SCL are high
    while(i2c->ISR & I2C_ISR_BUSY) {}

    // Kick off the transaction:
    //   Set the device address (SADD)
    //   Set the number of bytes to transfer (NBYTES),
    //   and auto-send STOP once we've sent that many bytes.
    uint32_t cr2 = I2C_CR2_AUTOEND |
                   len << I2C_CR2_NBYTES_Pos |
                   device_address << 1 | // Only bits 7:1 matter for 7-bit address
                   I2C_CR2_START;
    i2c->CR2 = cr2;

    // Error handling:
    // We get NCK on the address
    // We get NCK on the data, or something goes terribly wrong

    // Now feed bytes to be transmitted, one at a time
    for(int i = 0; i < len; i++){
        //if(i2c->ISR & I2C_ISR_NACKF){
        //    break;
        //}
        while(!(i2c->ISR & I2C_ISR_TXIS)) {} // Wait until it's ready for the next byte
        i2c->TXDR = data[i]; // TXIS is cleared when we write the next byte
    }

    // Wait for the transaction to complete (stop bit flag to be set)
    while(!(i2c->ISR & I2C_ISR_STOPF)) {}
    i2c->ICR = I2C_ICR_STOPCF;

    // Check whether we got ACK or NCK
    if(i2c->ISR & I2C_ISR_NACKF){
        i2c->ICR = I2C_ICR_NACKCF; // Clear the NCK flag
        return false;
    }

    return true; // Success
}


bool i2c_read(I2C_TypeDef* i2c, unsigned char device_address, unsigned char* data, unsigned char len)
{
    // Wait until the I2C peripheral is not busy
    while(i2c->ISR & I2C_ISR_BUSY) {}

    // Kick off the transaction:
    //   Set the device address (SADD)
    //   Set the number of bytes to transfer (NBYTES), and auto-send STOP
    //   once we've sent that many bytes.
    uint32_t cr2 = I2C_CR2_AUTOEND |
                   I2C_CR2_RD_WRN | // Read mode
                   len << I2C_CR2_NBYTES_Pos |
                   device_address << 1 | // Only bits 7:1 matter for 7-bit address
                   I2C_CR2_START;
    i2c->CR2 = cr2;

    // Now read bytes one at a time
    for(int i = 0; i < len; i++){
        while(!(i2c->ISR & I2C_ISR_RXNE)) {} // Wait until data is available
        data[i] = i2c->RXDR;
    }

    // Check whether we got ACK or NCK
    if(i2c->ISR & I2C_ISR_NACKF){
        i2c->ICR = I2C_ICR_NACKCF; // Clear the NCK flag
        return false;
    }

    return true; // If we got here, success
}