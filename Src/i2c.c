/*
 * i2c.c
 *
 *  Created on: Jun 17, 2024
 *      Author: amberyang
 */
#include "i2c.h"
#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>


void I2C_init(void) {

    // Reset I2C1
    RCC->APB1RSTR |= RCC_APB1RSTR_I2C1RST;
    RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;

    // Set the peripheral clock frequency to be the default 16 MHz
    I2C1 -> CR2 = 16;

    // Set SCL frequency to be 100 kHz (Sm mode)
    I2C1 -> CCR = 80;

    // Set max rise time
    I2C1 -> TRISE = 17;

    // Enable ACK
    I2C1->CR1 |= I2C_CR1_ACK;

    // Enable I2C1
    I2C1->CR1 |= I2C_CR1_PE;

}


// I2C start condition
void I2C_start(void){

	// Generate a start condition
	I2C1 -> CR1 |= I2C_CR1_START;

	// Keep looping till the SB flag is set (i.e. start condition generated)
	uint32_t SB_timeout = TIMEOUT;
	while(!(I2C1 -> SR1 & I2C_SR1_SB)){
		if(--SB_timeout == 0){
			printf("Timeout waiting for SB flag to be set in start condition. \n");
			return;
		}
	}

	// Clear SB flag by reading SR1
	(void)I2C1->SR1;
	printf("Start condition generated. \n");

}

// I2C stop condition
void I2C_stop(void){

	// Generate a stop condition
	I2C1 -> CR1 |= I2C_CR1_STOP;
	printf("Stop condition generated. \n");

}

// I2C 7-bit address transmission
void I2C_write_address(uint8_t address){

	uint8_t slave_addr = address << 1;
	slave_addr &= ~(1); // master write to slave: lsb = 0

	// Write DR register with the slave address
	I2C1 -> DR = slave_addr;

	uint32_t ADDR_timeout = TIMEOUT;

	// Keep looping till ADDR flag is set (i.e. address sent)
	while(!(I2C1 -> SR1 & I2C_SR1_ADDR)) {

		if (--ADDR_timeout == 0){
			printf("Timeout waiting for ADDR flag to be set in address transmission. \n");
			return;
			}
		}

	// Clear ADDR flag by reading SR1 and SR2
	(void)I2C1->SR1;
	(void)I2C1->SR2;

	printf("I2C address sent\n");

}


// I2C data transmission
void I2C_write_data(uint8_t data){

	uint32_t TXE_timeout = TIMEOUT;
	// Keep looping till TXE flag is set (i.e. data register is empty)
	while(!(I2C1 -> SR1 & I2C_SR1_TXE)){

		if(--TXE_timeout == 0){
			printf("Timeout waiting for TXE flag to be set in data transmission. \n");
			return;
		}
	}

	// Write data (TXE flag is cleared by writing DR register)
	I2C1 -> DR = data;

	uint32_t BTF_timeout = TIMEOUT;
	// Keep looping till BTF flag is set (i.e. byte transfer is finished)
	while(!(I2C1 -> SR1 & I2C_SR1_BTF)){

		if (--BTF_timeout == 0){
			printf("Timeout waiting for BEF flag to be set in data transmission. \n");
			return;
		}
	}

	printf("I2C data sent: 0x%02X\n", data);

}
