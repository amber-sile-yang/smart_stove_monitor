#include "stm32f4xx.h"
#include <stdio.h>
#include "gpio.h"
#include "i2c.h"
#include "adc.h"
#include "helper.h"
#include "lcd.h"

#define LCD_ADDRESS 0x27

int main(void) {
    // System initialization
    GPIO_init();
    I2C_init();

    while (1) {
        I2C_start();
        I2C_write_address(LCD_ADDRESS);
        I2C_write_data(0xA5);
        I2C_stop();
        timer_delay_ms(1000);
    }
}


