/*
 * project.h
 *
 *  Created on: Jan 8, 2023
 *      Author: bruce
 */

#ifndef INC_PROJECT_H_
#define INC_PROJECT_H_

#include "UART.h"
#include "printf.h"

void run_project();
void display_menu();
void read_input_string(char *buffer, uint32_t max_length);

#endif /* INC_PROJECT_H_ */
