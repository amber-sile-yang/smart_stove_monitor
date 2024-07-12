/*
 * oled.c
 *
 *  Created on: Jun 29, 2024
 *      Author: amberyang
 */


#include "oled.h"
#include "i2c.h"
#include "font.h"
#include "helper.h"
#include <stdint.h>
#include <stdio.h>

#define OLED_ADDRESS 0x3C
#define OLED_CMD_BYTE 0x00 // Co bit = 0, D/C# bit = 0
#define OLED_DATA_BYTE 0x40 // Co bit = 0, D/C# bit = 1

#define OLED_DISPLAY_OFF 0xAE
#define OLED_DISPLAY_ON 0xAF

#define OLED_SET_MUX_RATIO 0xA8
#define OLED_SET_MUX_RATIO_DEFAULT 0x3F // 1:64

#define OLED_SET_DISPLAY_OFFSET 0xD3
#define OLED_SET_DISPLAY_OFFSET_DEFAULT 0x00
#define OLED_SET_DISPLAY_START_LINE_DEFAULT 0x40

#define OLED_SET_SEGMENT_REMAP_REMAPPED_MODE 0xA1
#define OLED_SET_SEGMENT_REMAP_DEFAULT_MODE 0xA0
#define OLED_SET_COM_OUTPUT_SCAN_DIRECTION_REMAPPED_MODE 0xC8
#define OLED_SET_COM_OUTPUT_SCAN_DIRECTION_DEFAULT_MODE 0xC0

#define OLED_SET_COM_PINS_HW_CONFIG 0xDA
#define OLED_SET_COM_PINS_HW_CONFIG_SEQUENTIAL 0x12

#define OLED_SET_MEM_ADDR_MODE 0x20
#define OLED_SET_MEM_ADDR_MODE_PAGE 0x02
#define OLED_SET_PAGE_START_ADDR_0 0xB0
#define OLED_SET_LOWER_COL_START_ADDR_DEFAULT 0x00
#define OLED_SET_HIGHER_COL_START_ADDR_DEFAULT 0x10

#define OLED_SET_CONTRAST_CTRL 0x81
#define OLED_SET_CONTRAST_CTRL_DEFAULT 0x7F

#define OLED_ENTIRE_DISPLAY_ON 0xA4
#define OLED_SET_NORMAL_DISPLAY 0xA6

#define OLED_SET_OSC_FREQ 0xD5
#define OLED_SET_OSC_FREQ_DEFAULT 0x80

#define OLED_SET_CHARGE_PUMP 0x8D
#define OLED_ENABLE_CHARGE_PUMP 0x14

#define OLED_WIDTH 128
#define OLED_HEIGHT 64


void OLED_send_cmd(uint8_t cmd) {
    I2C_start();
    I2C_write_address(OLED_ADDRESS);
    I2C_write_data(OLED_CMD_BYTE);
    I2C_write_data(cmd);
    I2C_stop();
}

void OLED_send_data(uint8_t data) {
    I2C_start();
    I2C_write_address(OLED_ADDRESS);
    I2C_write_data(OLED_DATA_BYTE);
    I2C_write_data(data);
    I2C_stop();
}

void OLED_init(void) {

    timer_delay_ms(100); // Ensure pwr is stable
    OLED_send_cmd(OLED_DISPLAY_OFF);
    OLED_send_cmd(OLED_SET_MUX_RATIO);
    OLED_send_cmd(OLED_SET_MUX_RATIO_DEFAULT); // 1:64
    OLED_send_cmd(OLED_SET_DISPLAY_OFFSET);
    OLED_send_cmd(OLED_SET_DISPLAY_OFFSET_DEFAULT); // No offset
    OLED_send_cmd(OLED_SET_DISPLAY_START_LINE_DEFAULT); // Start line 0
    OLED_send_cmd(OLED_SET_SEGMENT_REMAP_REMAPPED_MODE);
    OLED_send_cmd(OLED_SET_COM_OUTPUT_SCAN_DIRECTION_REMAPPED_MODE);
    OLED_send_cmd(OLED_SET_COM_PINS_HW_CONFIG);
    OLED_send_cmd(OLED_SET_COM_PINS_HW_CONFIG_SEQUENTIAL);

    OLED_send_cmd(OLED_SET_MEM_ADDR_MODE);
    OLED_send_cmd(OLED_SET_MEM_ADDR_MODE_PAGE);
    OLED_send_cmd(OLED_SET_LOWER_COL_START_ADDR_DEFAULT);
    OLED_send_cmd(OLED_SET_HIGHER_COL_START_ADDR_DEFAULT);

    OLED_send_cmd(OLED_SET_CONTRAST_CTRL);
    OLED_send_cmd(OLED_SET_CONTRAST_CTRL_DEFAULT);
    OLED_send_cmd(OLED_ENTIRE_DISPLAY_ON);
    OLED_send_cmd(OLED_SET_NORMAL_DISPLAY);
    OLED_send_cmd(OLED_SET_OSC_FREQ);
    OLED_send_cmd(OLED_SET_OSC_FREQ_DEFAULT);
    OLED_send_cmd(OLED_SET_CHARGE_PUMP);
    OLED_send_cmd(OLED_ENABLE_CHARGE_PUMP);
    OLED_send_cmd(OLED_DISPLAY_ON);

}


// Clear entire display
void OLED_clear(void) {
	// Loop through the entire display
    for (uint16_t i = 0; i < OLED_WIDTH * OLED_HEIGHT / 8; i++) {
    	// Turn the pixel off
        OLED_send_data(0x00);
    }
}



/*
void OLED_clear(void) {
    for (uint8_t page = 0; page < 8; page++) {
        OLED_set_cursor(0, page);
        for (uint8_t col = 0; col < 128; col++) {
            OLED_send_data(0x00);
        }
    }
}
*/



void OLED_print_str(const char* str) {
	// Loop through all the chars pointed by str till '\0' is reached
    while (*str) {
    	// Loop through 5 columns defined in font array
        for (uint8_t i = 0; i < 5; i++) {

        	// row 0 of the font array is 'space', which has ASCII number of 32
            OLED_send_data(font[*str - 32][i]);
        }
        // Add space between each character
        OLED_send_data(0x00);
        str++;
    }
}


// Set cursor position for where the next data byte will be written
// Note: GDDRAM has a total of 7 pages with 8 rows & 128 columns per page
void OLED_set_cursor(uint8_t col, uint8_t page) {
    OLED_send_cmd(OLED_SET_PAGE_START_ADDR_0 + page);

    // Set higher col start address (higher nibble)
    OLED_send_cmd(((col & 0xF0) >> 4) | OLED_SET_HIGHER_COL_START_ADDR_DEFAULT);

    // Set lower col start address (lower nibble)
    OLED_send_cmd((col & 0x0F) | OLED_SET_LOWER_COL_START_ADDR_DEFAULT);
}
