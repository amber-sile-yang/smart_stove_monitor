/*
 * adc.c
 *
 *  Created on: Jun 17, 2024
 *      Author: amberyang
 */
#include "stm32f4xx.h"
#include "adc.h"
#include <stdint.h>


// ADC initialization
void ADC_init(void) {

    // Enable ADC1 clock
    RCC -> APB2ENR |= RCC_APB2ENR_ADC1EN;

    // Set regular channel sequence length to 1 conversion since we only have 1 sensor
    ADC1 -> SQR1 &= ~ADC_SQR1_L;

    // Set ADC1's 1st conversion in regular sequence to ch4 (mapped to PA4)
    ADC1 -> SQR3 = 4;

    // Set ch4 sample time to be 84 cycles
    ADC1 -> SMPR2 |= ADC_SMPR2_SMP4_2;

    // Enable ADC1
    ADC1 -> CR2 |= ADC_CR2_ADON;
}

// Start ADC Conversion
void start_ADC_conversion(void) {

    // Continuous conversion mode
    ADC1 -> CR2 |= ADC_CR2_CONT;

    // Start conversion of regular channels
    ADC1 -> CR2 |=  ADC_CR2_SWSTART;

}

// ADC watchdog (AWD) initialization
void ADC_watchdog_init(void) {

    // Set high threshold value corresponding to 30°C
    ADC1 -> HTR = 1109;

    // Low threshold value to be 0
    ADC1 -> LTR = 0;

	// Select 12-bit resolution
	ADC1 -> CR1 &= ~ADC_CR1_RES;

	// AWD enabled on regular channels
	ADC1 -> CR1 |= ADC_CR1_AWDEN;

    // AWD enabled on a single channel
    ADC1 -> CR1 |= ADC_CR1_AWDSGL;

    // Enable AWD interrupt
    ADC1 -> CR1 |= ADC_CR1_AWDIE;

    // Select ch4 for AWD
    ADC1 -> CR1 &= ~(ADC_CR1_AWDCH);
    ADC1 -> CR1 |= ADC_CR1_AWDCH_2;

    // Interrupt enable for End of Conversion
    ADC1 -> CR1 |=ADC_CR1_EOCIE;

    // Enable ADC interrupt in NVIC
    NVIC_EnableIRQ(ADC_IRQn);
}

// ADC interrupt handler
void ADC_IRQHandler(void) {

    // Check if AWD flag is set
    if (ADC1 -> SR & ADC_SR_AWD) {

    	// For debug purpose
    	//printf("SR value before clearing AWD flag: ADC1->SR = %lu\n", ADC1->SR);

        // Clear the flag before handling the event
        ADC1 -> SR &= ~ADC_SR_AWD;

        // For debug purpose
        //printf("SR value after clearing AWD flag: ADC1->SR = %lu\n", ADC1->SR);

        // If ADC value is outside the threshold: turn on the buzzer & LED
        // Otherwise, ensure buzzer & LED are off state
        uint16_t adc_value = read_ADC();
        if (adc_value > 1109 || adc_value < 0) {

        	// Turn on the buzzer connected to PA5
        	GPIOA -> ODR |= GPIO_ODR_ODR_5;

        	// Turn on the LED connected to PA6
        	GPIOA -> ODR |= GPIO_ODR_ODR_6;
        }
        else {

        	// Turn off the buzzer connected to PA5
        	GPIOA -> ODR &= GPIO_ODR_ODR_5;

        	// Turn off the LED connected to PA6
        	GPIOA -> ODR &= GPIO_ODR_ODR_6;
        }

        // For debug purpose
        //printf("ADC value when interrupt is triggered. ADC Value: %lu\n", ADC1->DR);
    }

    // Check if the EOC interrupt flag is set
    if (ADC1->SR & ADC_SR_EOC) {

        // Clear the EOC interrupt flag
        ADC1->SR &= ~ADC_SR_EOC;

        // No specific handling
    }
}


// Read ADC value
uint16_t read_ADC(void) {

    // Keep looping till conversion completes on regular channel
    while (!(ADC1->SR & ADC_SR_EOC));

    // Return ADC value
    return ADC1->DR;

}

// Read temperature
float read_temp(uint16_t adc_value){

	float temperature = (adc_value * 3.3 / 4095.0 - 0.5) * 100.0;
	return temperature;

}
