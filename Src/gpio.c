/*
 * gpio.c
 *
 *  Created on: Jun 17, 2024
 *      Author: amberyang
 */


#include "gpio.h"



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

    //-----------------------------I2C CONFIG--------------------------------

    // Enable clock for I2C1
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    // Enable clock for GPIOB
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    // Set PB6 & PB7 to alternate function (AF) Mode
    GPIOB -> MODER &= ~(GPIO_MODER_MODER6);
    GPIOB -> MODER |= GPIO_MODER_MODER6_1;
    GPIOB -> MODER &= ~(GPIO_MODER_MODER7);
    GPIOB -> MODER |= GPIO_MODER_MODER7_1;

    // Set PB6 to AF4 (SCL) and PB7 to AF4 (SDA)
    GPIOB -> AFR[0] |= (4 << 24)| (4 << 28);

    // Set PB6 & PB7 output type to be open-drain
    GPIOB->OTYPER |= (GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7);

    // Enable pull-up resistors for SCL & SDA lines
    GPIOB -> PUPDR &= ~(GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7);
    GPIOB -> PUPDR |= GPIO_PUPDR_PUPDR6_0 | GPIO_PUPDR_PUPDR7_0;


}
