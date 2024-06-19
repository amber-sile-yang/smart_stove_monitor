#include "stm32f4xx.h"
#include <stdio.h>
#include "gpio.h"
#include "i2c.h"
#include "adc.h"
#include "helper.h"

// Default I2C address is 0x27 or 0x3F
#define LCD_ADDRESS 0x27

// Function prototypes
void I2C_master_test(void);


int main(void) {
    //SystemInit();

    GPIO_init();
    ADC_init();
    ADC_watchdog_init();
    start_ADC_conversion();
    I2C_init();


    while(1){

    	// Toggle LED connected to PA1 to ensure main loop is running
    	GPIOA -> ODR ^= GPIO_ODR_ODR_1;
    	timer_delay_ms(1000);

    	// Perform I2C master test
    	I2C_master_test();

    }
}



void I2C_master_test(void){

	// Generate start condition
	I2C_start();

	// Send dummpy slave address
	I2C_write_address(LCD_ADDRESS);

	// Send dummy data
	uint8_t data = 0xFF;
	I2C_write_data(data);

	// Generate stop condition
	I2C_stop();
}
