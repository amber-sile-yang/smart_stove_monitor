#include "stm32f4xx.h"
#include <stdio.h>

// Function prototypes
void GPIO_init(void);
void timer_delay_ms(uint32_t delay_ms);
void ADC_init(void);
void ADC_watchdog_init(void);
void start_ADC_conversion(void);
uint16_t read_ADC(void);

int main(void) {
    //SystemInit();

    GPIO_init();
    ADC_init();
    ADC_watchdog_init();
    start_ADC_conversion(); // Start the ADC conversion

    while(1){

        // Toggle PA1 to verify if the main loop is running
        GPIOA->ODR ^= GPIO_ODR_ODR_1; // Toggle PA1
        timer_delay_ms(1000); // 1 second delay

        // Read ADC and temperature value and print it using SWV
        uint16_t adc_value = read_ADC();
        printf("ADC Value before interrupt is triggered: %u\n", adc_value);

    }
}

// GPIO initialization
void GPIO_init(void) {

    // Enable clock for GPIOA
    RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // Set PA0 as input for motion sensor: bits 1:0 of MODER set to 00
    GPIOA -> MODER &= ~(GPIO_MODER_MODER0);

    // Set PA1 as output for LED: bits 3:2 set to 01
    GPIOA -> MODER |= GPIO_MODER_MODER1_0;
    GPIOA -> MODER &= ~(GPIO_MODER_MODER1_1);

    // Set PA4 as analog input for temp sensor: bits 9:8 set to 11
    GPIOA -> MODER |= GPIO_MODER_MODER4;

    // Set PA5 as output for buzzer: bits 11:10 set to 01
    GPIOA -> MODER |= GPIO_MODER_MODER5_0;
    GPIOA -> MODER &= ~(GPIO_MODER_MODER5_1);

    // Set PA6 as output for debug LED: bits 13:12 set to 01
    GPIOA -> MODER |= GPIO_MODER_MODER6_0;
    GPIOA -> MODER &= ~(GPIO_MODER_MODER6_1);

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

    // Set high threshold value corresponding to 35°C
    ADC1 -> HTR = 1160;

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

// ADC interrupt handler implementation
void ADC_IRQHandler(void) {

    // Check if AWD flag is set
    if (ADC1 -> SR & ADC_SR_AWD) {

    	printf("SR value before clearing AWD flag: ADC1->SR = %lu\n", ADC1->SR);

        // Clear the flag before handling the event
        ADC1 -> SR &= ~ADC_SR_AWD;

        printf("SR value after clearing AWD flag: ADC1->SR = %lu\n", ADC1->SR);

        // If ADC value is outside the threshold: turn on the buzzer & LED
        uint16_t adc_value = read_ADC();
        if (adc_value > 1160 || adc_value < 0) {

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

        // Print ADC value when interrupt is triggered
        printf("ADC value when interrupt is triggered. ADC Value: %lu\n", ADC1->DR);
    }

    // Check if the EOC (End of Conversion) interrupt flag is set
    if (ADC1->SR & ADC_SR_EOC) {
        // Clear the EOC interrupt flag
        ADC1->SR &= ~ADC_SR_EOC;

        // Handle the EOC interrupt if needed
        // For this example, we don't need to do anything specific for EOC
    }

}


// Read ADC value
uint16_t read_ADC(void) {

    // Keep looping till conversion completes on regular channel
    while (!(ADC1->SR & ADC_SR_EOC));

    // Return ADC value
    return ADC1->DR;

}


// Redirect printf to ITM
int _write(int file, char *ptr, int len) {

    for (int i = 0; i < len; i++) {

        ITM_SendChar(*ptr++);
    }

    return len;
}
