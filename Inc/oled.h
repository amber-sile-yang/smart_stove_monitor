/*
 * oled.h
 *
 *  Created on: Jun 29, 2024
 *      Author: amberyang
 */

#ifndef OLED_H_
#define OLED_H_
#include <stdint.h>

void OLED_init(void);
void OLED_send_cmd(uint8_t command);
void OLED_send_data(uint8_t data);
void OLED_clear(void);
void OLED_set_cursor(uint8_t x, uint8_t y);
void OLED_print_str(const char* str);


#endif /* OLED_H_ */
