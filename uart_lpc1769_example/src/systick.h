/*
 * systick.h
 *
 *  Created on: 19 mar. 2021
 *      Author: ealegremendoza
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_
#include "headers.h"

#define CCLK 	100000000
void SysTick_Init(uint32_t frecuencia);
void SysTick_Handler(void);


#endif /* SYSTICK_H_ */
