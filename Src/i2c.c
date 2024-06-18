/*
 * i2c.c
 *
 *  Created on: Jun 17, 2024
 *      Author: amberyang
 */
#include "i2c.h"
#include "stm32f4xx.h"
#include <stdint.h>


// I2C initialization
void I2C_init(void){

	// Enable clock for I2C connected to APB1
	RCC -> APB1ENR |= RCC_APB1ENR_I2C1EN;

	//!!!!! FACT CHECK IF THE DEFAULT PRESCALER FOR APB1 IS 1 OR 4!!!!!!!!!
    // Reset I2C1
    RCC -> APB1RSTR |= RCC_APB1RSTR_I2C1RST;
    RCC -> APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;

	// Set the peripheral clock frequency to be the default 16MHz
	I2C1 -> CR2 = 16;

	// Set SCL frequency to be 100 kHz (Sm mode)
	I2C1 -> CCR = 80;

	// Set max rise time
	I2C1 -> TRISE = 17;

	// Enable I2C1 peripheral
	I2C1 -> CR1 |= I2C_CR1_PE;

}

// I2C start condition
void I2C_start(void){

	// Generate a start condition
	I2C1 -> CR1 |= I2C_CR1_START;

	// Keep looping till the SB flag is set (i.e. start condition generated)
	while(!(I2C1 -> SR1 & I2C_SR1_SB));

	// Clear SB flag by reading SR1
	(void)I2C1->SR1;

}

// I2C stop condition
void I2C_stop(void){

	// Generate a stop condition
	I2C1 -> CR1 |= I2C_CR1_STOP;

}

// I2C 7-bit address transmission
void I2C_write_address(uint8_t address){

	// Write DR register with the slave address
	I2C1 -> DR = address;

	// Keep looping till ADDR flag is set (i.e. address sent)
	while(!(I2C1 -> SR1 & I2C_SR1_ADDR));

	// Clear ADDR flag by reading SR1 and SR2
	(void)I2C1->SR1;
	(void)I2C1->SR2;

}

// I2C data transmission
void I2C_write_data(uint8_t data){

	// Keep looping till TXE flag is set (i.e. data register is empty)
	while(!(I2C1 -> SR1 & I2C_SR1_TXE));

	// Write data (TXE flag is cleared by writing DR register)
	I2C1 -> DR = data;

	// Keep looping till BEF flag is set (i.e. byte transfer is finished)
	while(!(I2C1 -> SR1 & I2C_SR1_BTF));

}
