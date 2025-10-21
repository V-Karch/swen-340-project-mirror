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

typedef struct {
	// Starting from 0xE000E010
	uint32_t SYST_CSR;
	uint32_t SYST_RVR;
	uint32_t SYST_CVR;
	uint32_t SYST_CALIB;
} SysTick;

// This fuction is to create delay using SysTick timer counter
void delay_systick();

#endif /* INC_SYSTICK_H_ */
