/*
 * systick.h
 *
 *  Created on: Jan 8, 2023
 *      Author: bruce
 */

#ifndef INC_SYSTICK_H_
#define INC_SYSTICK_H_

// This function is to Initialize SysTick registers
void init_systick();

// This fuction is to create delay using SysTick timer counter
void delay_systick();

#endif /* INC_SYSTICK_H_ */
