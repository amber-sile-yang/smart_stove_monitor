/*
 * i2c.h
 *
 *  Created on: Jun 17, 2024
 *      Author: amberyang
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>

void I2C_init(void);
void I2C_start(void);
void I2C_stop(void);
void I2C_write_address(uint8_t address);
void I2C_write_data(uint8_t data);


#endif /* I2C_H_ */
