/*
 * helper.c
 *
 *  Created on: Jun 17, 2024
 *      Author: amberyang
 */
#include "helper.h"
#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>


volatile uint32_t motion_timer = 0;
volatile uint32_t sw_press_duration = 0;
volatile uint8_t system_armed = 1; // System is armed by default
volatile uint32_t sys_rearm_timer = 0;
// Auto-rearm timer for 1 hour (3600,000 ms)
#define REARM_THRESHOLD_MS 3600000
//For test purpose: Auto-rearm timer for 20s (20000 ms)
//#define REARM_THRESHOLD_MS 20000


// Configure SysTick timer to generate an interrupt every 1ms
void SysTick_Init(void) {
    // Configure SysTick timer to generate an interrupt every 1ms
    SysTick_Config(SystemCoreClock / 1000);
}

// SysTick ISR: executes each time SysTick timer expires (interrupt triggered)
void SysTick_Handler(void) {
    if (motion_timer > 0) {
        motion_timer--;
    }

    // Check if the button connected to PA2 is pressed
    if (!(GPIOA->IDR & GPIO_IDR_IDR_2)) {
        sw_press_duration++;
        // 3s-press to toggle system state
        if (sw_press_duration >= 3000) {
            system_armed = !system_armed;
            // Reset sw_press_duration after each 3s-press
            sw_press_duration = 0;
            // Start system re-arm timer if the system is disarmed
            if (!system_armed) {
                sys_rearm_timer = REARM_THRESHOLD_MS;
            } else {
                sys_rearm_timer = 0;
            }
        }
    } else {
        sw_press_duration = 0;
    }

    // Count down REARM_THRESHOLD_MS till 0 to re-arm the system
    if (sys_rearm_timer > 0) {
        sys_rearm_timer--;
        if (sys_rearm_timer == 0 && !system_armed) {
            system_armed = 1;
        }
    }
}



// ms delay using timer
void timer_delay_ms(uint32_t delay_ms) {
    // Enable TIM2 clock
    RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Set 1kHz (from 16MHz source) to achieve 1ms for each count
    TIM2 -> PSC = (16000000 / 1000) - 1;

    // Load the desired delay
    TIM2 -> ARR = delay_ms;

    // Start counting from 0
    TIM2 -> CNT = 0;

    // Enable the counter: bit 0 of CR1 is set
    TIM2 -> CR1 |= TIM_CR1_CEN;

    // Keep looping while Update Interrupt Flag (UIF) is not set in Status Register (SR)
    while (!(TIM2 -> SR & TIM_SR_UIF)) {}

    // Clear UIF after it is set
    TIM2 -> SR &= ~(TIM_SR_UIF);
}




// Redirect printf to ITM
int _write(int file, char *ptr, int len) {

    for (int i = 0; i < len; i++) {

        ITM_SendChar(*ptr++);
    }

    return len;
}


void format_temp_str(int temp, char *str) {
    sprintf(str, " %d C", temp);
}


