/*
 * activity7.c
 *
 *  Created on: Oct 2, 2025
 *      Author: luna
 */

// Write a function that turns on your LED when the user types O
// Turn off when they press F

#include "LED.h"
#include "UART.h"

void run_activity_7() {
    char c;
    LED_Init();

    while (1) {
        c = USART_Read(USART2);
        if (c == 'O') {
            LED_On();
        } else if (c == 'F') {
            LED_Off();
        }
    }
}
