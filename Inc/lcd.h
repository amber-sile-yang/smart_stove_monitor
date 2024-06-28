/*
 * lcd.h
 *
 *  Created on: Jun 27, 2024
 *      Author: amberyang
 */

#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>

#define LCD_ADDRESS 0x27

void LCD_send_command(uint8_t cmd);
void LCD_send_data(uint8_t data);
void LCD_init(void);
void LCD_send(uint8_t data, uint8_t mode);
void LCD_write_4bits(uint8_t data);


#endif /* LCD_H_ */
