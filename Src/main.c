/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Amber Sile Yang
 * @title          : smart stove monitor
 * @title          : 31-may-2024
 ******************************************************************************
 * @about
 * this project is designed to alert home owner when the stove is left unattended
 *
 * bare metal (register-level) programming approach is used to interface the
 * STM32f407G-DISC1 MCU with HC-SR501 PIR motion sensor, DS18B20 temperature
 * sensor, LCD1602 display, LED, and buzzer
 *
 ******************************************************************************
 */

#include "stm32f4xx.h"

// Function prototypes
void GPIO_Init(void);
void timer_delay_ms(uint32_t delay_ms);

int main(void) {

    //SystemInit();

    GPIO_Init();

    while(1){

    	// turn on the LED if the motion is detected
    	if(GPIOA -> IDR & GPIO_IDR_IDR_0){
    		GPIOA -> ODR |= GPIO_ODR_ODR_1;
    	}
    	else{
    		GPIOA -> ODR &= ~GPIO_ODR_ODR_1;
    	}

    	// 1s delay
    	//timer_delay_ms(1000);
    }

}

// SystemInit
// ...

// GPIO initialization
void GPIO_Init(void) {

	// enable clk for GPIOA: bit 0 of AHB1ENR is set
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	// set PA0 as i/p for motion sensor: bits 1:0 of MODER set to 00
	GPIOA -> MODER &= ~(GPIO_MODER_MODER0);

	// set PA1 as o/p for LED: bits 3:2 set to 01
	GPIOA -> MODER |= GPIO_MODER_MODER1_0;


}

// ms delay using timer
void timer_delay_ms(uint32_t delay_ms){

	//enable tim2 clk
	RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN;

	// set 1kHz (from 16MHz src) to achieve 1/1kHz = 1ms for each count
	// CK_CNT = fCK_PSC / (PSC[15:0] + 1)
	// 1kHz = 16MHz / (prescale_value + 1 )
	TIM2 -> PSC = (16000000/1000)-1;

	// load the desired delay
	TIM2 -> ARR = delay_ms;

	// start counting from 0
	TIM2 -> CNT = 0;

	// enable the counter: bit 0 of CR1 is set
	TIM2 -> CR1 |= TIM_CR1_CEN;

	// keep looping while Update Interrupt Flag (UIF) is not set in Status Register (SR)
	while(!(TIM2 -> SR & TIM_SR_UIF)){}

	// clc UIF after it is set
	TIM2 -> SR &= ~(TIM_SR_UIF);

}




















