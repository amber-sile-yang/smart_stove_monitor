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


int main(void) {
    //SystemInit();
    GPIO_Init();

}

// SystemInit
// ...

// GPIO_Init
void GPIO_Init(void) {

	// enable clk for GPIOA
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	// set PA0 as i/p for motion sensor


}
