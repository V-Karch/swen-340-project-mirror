/*
 * systick.h
 *
 *  Created on: Jan 8, 2023
 *      Author: bruce
 */

#ifndef INC_SYSTICK_H_
#define INC_SYSTICK_H_

#include "stdint.h"

// This function is to Initialize SysTick registers
void init_systick();
uint32_t get_total_count();

typedef struct {
	// Starting from 0xE000E010
	uint32_t SYST_CSR;
	uint32_t SYST_RVR;
	uint32_t SYST_CVR; // Current value
	uint32_t SYST_CALIB;
} SYSTICK;

// This fuction is to create delay using SysTick timer counter
void delay_systick();

// Blocking delay in microseconds
void delay_us(uint32_t microseconds);

// Blocking delay in milliseconds
void delay_ms(uint32_t milliseconds);

uint32_t get_total_count();

#endif /* INC_SYSTICK_H_ */
