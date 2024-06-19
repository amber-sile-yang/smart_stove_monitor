/*
 * lcd.h
 *
 *  Created on: Jun 18, 2024
 *      Author: amberyang
 */


#ifndef LCD_H_
#define LCD_H_

#define LCD_CMD_CLEAR_DISPLAY 0x01
#define LCD_CMD_RETURN_HOME 0x02
#define LCD_CMD_ENTRY_MODE_SET 0x04
#define LCD_CMD_DISPLAY_CONTROL 0x08
#define LCD_CMD_CURSOR_SHIFT 0x10
#define LCD_CMD_FUNCTION_SET 0x20
#define LCD_CMD_SET_CGRAM_ADDR 0x40
#define LCD_CMD_SET_DDRAM_ADDR 0x80

#define LCD_ENTRY_LEFT 0x02
#define LCD_ENTRY_SHIFT_DECREMENT 0x00
#define LCD_ENTRY_MODE (LCD_ENTRY_LEFT | LCD_ENTRY_SHIFT_DECREMENT)

#define LCD_DISPLAY_ON 0x04
#define LCD_CURSOR_OFF 0x00
#define LCD_BLINK_OFF 0x00
#define LCD_DISPLAY_CONTROL (LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF)

#define LCD_FUNCTION_2LINE 0x08
#define LCD_FUNCTION_5x8DOTS 0x00
#define LCD_FUNCTION_SET (LCD_FUNCTION_2LINE | LCD_FUNCTION_5x8DOTS)

#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE_BIT 0x04

// Default I2C address is 0x27 or 0x3F
#define LCD_ADDRESS 0x27

#include <stdint.h>

void I2C_LCD_write(uint8_t data, uint8_t flags);
void LCD_send_command(uint8_t command);
void LCD_send_data(uint8_t data);
void LCD_init(void);
void LCD_clear(void);
void LCD_set_cursor(uint8_t row, uint8_t col);
void LCD_print(char *str);
void display_temperature(float temperature);






#endif /* LCD_H_ */
