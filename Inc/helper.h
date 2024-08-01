/*
 * helper.h
 *
 *  Created on: Jun 17, 2024
 *      Author: amberyang
 */

#ifndef HELPER_H_
#define HELPER_H_

#include <stdint.h>

void timer_delay_ms(uint32_t delay_ms);
void timer_delay_us(uint32_t delay_us);
int _write(int file, char *ptr, int len);
void int_to_str(int num, char *str);
void format_temp_str(int temp, char *str);
void SysTick_Init(void);
void SysTick_Handler(void);





#endif /* HELPER_H_ */
