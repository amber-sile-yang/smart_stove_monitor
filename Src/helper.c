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


