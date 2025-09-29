/*
 * activity5.c
 *
 *  Created on: Sep 29, 2025
 *      Author: V-Karch
 */

#include "UART.h"
#include "printf.h"
#include "stdint.h"

void read_and_print_character() {
	printf("%c", USART_Read(USART2));
}

