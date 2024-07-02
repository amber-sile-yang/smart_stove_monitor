#include "stm32f4xx.h"
#include <stdio.h>
#include "gpio.h"
#include "i2c.h"
#include "adc.h"
#include "helper.h"
#include "oled.h"

int main(void) {
    // System initialization
    GPIO_init();
    I2C_init();
    ADC_init();
    OLED_init();
    start_ADC_conversion();
    OLED_clear(); // Clear the display at the start

    const char* msg = "Smart Stove Monitor:)";
    const char* temp_msg = "Current Temperature:";
	OLED_set_cursor(0, 0);
	OLED_print_str(msg);
	OLED_set_cursor(0, 2);
	OLED_print_str(temp_msg);

    while (1) {

    	int temp = read_temp();
    	char temp_str[16];
    	format_temp_str(temp, temp_str);
    	OLED_set_cursor(45, 5);
    	OLED_clear();
    	OLED_print_str(temp_str);
    	timer_delay_ms(5000);

    }

}
