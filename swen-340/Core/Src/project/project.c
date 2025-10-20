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
    char character = 0;

    while (1) {
        character = (char)USART_Read_Non_Blocking(USART2);

        if (character == 0) {
            continue;
        }

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

        if (i < max_length - 1) {
            buffer[i++] = character;
            printf("%c", character);
        }
    }

    buffer[i] = '\0';
    printf("\r\n");
}

void handle_user_input(char* buffer) {
	if (strcmp(buffer, "NEXT") == 0) {
		printf("\r\nAttempting to view next song...\r\n\r\n");
	} else if (strcmp(buffer, "PLAY") == 0) {
		printf("\r\nAttempting to play song...\r\n\r\n");
	} else if (strcmp(buffer, "PAUSE") == 0) {
		printf("\r\nAttempting to pause song...\r\n\r\n");
	} else if (strcmp(buffer, "STOP") == 0) {
		printf("\r\nAttempting to stop song...\r\n\r\n");
	} else {
		printf("\r\nInvalid command\r\n\r\n");
	}
}
