/*
 * lcd.c
 *
 *  Created on: Jun 27, 2024
 *      Author: amberyang
 */


#include "lcd.h"
#include "i2c.h"
#include "helper.h"
#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>


void LCD_init(void) {
    // Wait for power to stabilize
    timer_delay_ms(50);
    // Initialize the LCD in 4-bit mode
    LCD_send_command(0x30); // Wake up
    timer_delay_ms(5);
    LCD_send_command(0x30); // Wake up
    timer_delay_ms(1);
    LCD_send_command(0x30); // Wake up
    timer_delay_ms(1);
    LCD_send_command(0x20); // 4-bit mode
    timer_delay_ms(1);

    // LCD configuration
    LCD_send_command(0x28); // Function set: 4-bit mode, 2 lines, 5x8 dots
    LCD_send_command(0x08); // Display off
    LCD_send_command(0x01); // Clear display
    timer_delay_ms(2);
    LCD_send_command(0x06); // Entry mode set: increment automatically, no shift
    LCD_send_command(0x0C); // Display control: display on, cursor off, no blink
}

void LCD_send_command(uint8_t cmd) {
    LCD_send(cmd, 0);
}

void LCD_send_data(uint8_t data) {
    LCD_send(data, 1);
}

void LCD_send(uint8_t data, uint8_t mode) {
    uint8_t high_nibble = data & 0xF0;
    uint8_t low_nibble = (data << 4) & 0xF0;
    LCD_write_4bits(high_nibble | mode);
    LCD_write_4bits(low_nibble | mode);
}

void LCD_write_4bits(uint8_t data) {
    I2C_start();
    I2C_write_address(LCD_ADDRESS);
    I2C_write_data(data | 0x08); // Enable bit high
    I2C_write_data(data & ~0x08); // Enable bit low
    I2C_stop();
}
