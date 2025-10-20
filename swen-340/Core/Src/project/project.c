/*
 * project.c
 *
 *  Created on: Jan 8, 2023
 *      Author: bruce
 */

#include "project.h"

void display_menu() {
	printf("***REMOTE LED CONTROL MENU***\r\nAvailable User Commands\r\nNEXT - Show next song info\r\nPLAY - Play the song (LED ON)\r\nPAUSE - Pause the song (LED FLASH)\r\nSTOP - Stop the song(LED off)\r\n\r\n>>> ");
}

void read_input_string(char *buffer, uint32_t max_length) {
    uint32_t i = 0;
    char character;

    while (i < max_length - 1) {
        character = (char)USART_Read(USART2);
        USART_Write(USART2, (uint8_t *)&character, 1);
        // ^^ Echo typing back into terminal
        if (character == '\n' || character == '\r') {
            break;
        }

        buffer[i++] = character;
    }

    buffer[i] = '\0'; // null terminate
}
