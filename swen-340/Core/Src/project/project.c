/*
 * project.c
 *
 *  Created on: Jan 8, 2023
 *      Author: V-Karch
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

        if (character == '\n' || character == '\r') {
            break;
        }

        if (character == 0x08 || character == 0x7F) { // Backspaces
            if (i > 0) {
                i--;
                printf("\b \b");
            }
            continue;
        }

        buffer[i++] = character;
        printf("%c", character);
    }

    buffer[i] = '\0';
    printf("\r\n");
}
