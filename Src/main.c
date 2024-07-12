#include "stm32f4xx.h"
#include "system_stm32f4xx.h"
#include <stdio.h>
#include "gpio.h"
#include "i2c.h"
#include "adc.h"
#include "helper.h"
#include "oled.h"

extern volatile uint8_t adc_threshold_flag;

// Reset timer to 10 minute (600,000 ms)
//#define no_motion_threshold_ms 100000;
// For test purpose: Reset timer to 10s (10000 ms)
volatile uint32_t motion_timer = 0;
#define no_motion_threshold_ms 10000;
void SysTick_Handler(void) {
    if (motion_timer > 0) {
        motion_timer--;
    }
}

int main(void) {

    SystemInit();
    GPIO_init();
    // SysTick timer generates an interrupt every 1ms
    SysTick_Config(SystemCoreClock / 1000);
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
    	// Handle the event when ADC value is outside the threshold
        if (adc_threshold_flag) {
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

        // If no motion is detected, start counting down from the threshold value
        if (GPIOA->IDR & GPIO_IDR_IDR_0) {
            motion_timer = no_motion_threshold_ms;
        }

        int temp = read_temp();
        char temp_str[16];
        format_temp_str(temp, temp_str);
        OLED_set_cursor(45, 5);
        OLED_print_str(temp_str);
        timer_delay_ms(1000);
    }
}
