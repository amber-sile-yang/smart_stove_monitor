/*
 * adc.h
 *
 *  Created on: Jun 17, 2024
 *      Author: amberyang
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

void ADC_init(void);
void ADC_watchdog_init(void);
void start_ADC_conversion(void);
uint16_t read_ADC(void);
float read_temp(void);

#endif /* ADC_H_ */
