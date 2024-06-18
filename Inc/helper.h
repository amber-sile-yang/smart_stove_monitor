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
int _write(int file, char *ptr, int len);

#endif /* HELPER_H_ */
