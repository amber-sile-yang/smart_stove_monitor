#include "stm32f4xx.h"
#include "system_stm32f4xx.h"
#include <stdio.h>
#include "gpio.h"
#include "i2c.h"
#include "adc.h"
#include "helper.h"
#include "oled.h"

extern volatile uint8_t adc_threshold_flag;
extern volatile uint32_t motion_timer;
extern volatile uint8_t system_armed;
// Reset motion timer to 30 minute (1800000 ms)
#define NO_MOTION_THRESHOLD_MS 1800000;
// For test purpose: Reset motion timer to 10s (10000 ms)
//#define NO_MOTION_THRESHOLD_MS 10000;


int main(void) {

    SystemInit();
    GPIO_init();
    SysTick_Init();
    I2C_init();
    ADC_init();
    ADC_watchdog_init();
    OLED_init();
    OLED_clear();
    start_ADC_conversion();

    const char* msg = "Smart Stove Monitor:)";
    const char* temp_msg = "Current Temperature:";
    OLED_set_cursor(0, 0);
    OLED_print_str(msg);
    OLED_set_cursor(0, 2);
    OLED_print_str(temp_msg);

    while (1) {
    	// Handle the event when ADC value is outside the threshold while the system is armed
        if (adc_threshold_flag && system_armed) {
        	// Clear the flag after handling
        	adc_threshold_flag = 0;
            uint16_t adc_value = read_ADC();
            if ((adc_value > 1109 || adc_value < 0) && motion_timer == 0) {
                GPIOA -> ODR |= GPIO_ODR_ODR_5;  // Turn on the buzzer connected to PA5
                GPIOA -> ODR |= GPIO_ODR_ODR_6;  // Turn the LED connected to PA6
            } else {
                GPIOA -> ODR &= ~GPIO_ODR_ODR_5; // Turn off the buzzer connected to PA5
                GPIOA -> ODR &= ~GPIO_ODR_ODR_6; // Turn off the LED connected to PA6
            }
        }

        // Update the status LED based on system state
        if (system_armed) {
            GPIOA->ODR |= GPIO_ODR_ODR_1; // Turn on the status LED connected to PA1
        } else {
            GPIOA->ODR &= ~GPIO_ODR_ODR_1; // Turn off the status LED connected to PA1
        }

        // If no motion is detected, start counting down from the threshold value
        if (GPIOA->IDR & GPIO_IDR_IDR_0) {
            motion_timer = NO_MOTION_THRESHOLD_MS;
        }


        int temp = read_temp();
        char temp_str[16];
        format_temp_str(temp, temp_str);
        OLED_set_cursor(45, 5);
        OLED_print_str(temp_str);
        timer_delay_ms(500);
    }
}

