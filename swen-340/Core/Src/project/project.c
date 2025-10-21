/*
 * project.c
 *
 *  Created on: Jan 8, 2023
 *      Author: V-Karch
 */

#include "project.h"

char buffer_input[7];
uint8_t buffer_reset = 0;
uint32_t buffer_index = 0;

void display_menu() {
	printf("***REMOTE LED CONTROL MENU***\r\nAvailable User Commands\r\nNEXT - Show next song info\r\nPLAY - Play the song (LED ON)\r\nPAUSE - Pause the song (LED FLASH)\r\nSTOP - Stop the song(LED off)\r\n\r\n>>> ");
}

void read_input_string(char *buffer, uint32_t max_length) {
    char character = 0;

	character = (char)USART_Read_Non_Blocking(USART2);

	if (character == 0) {
		return;
	}

	if (character == '\n' || character == '\r') {
		buffer_reset = 1;
		printf("\r\n>>>");
		return;
	}

	if (character == 0x08 || character == 0x7F) { // Backspaces
		if (buffer_index > 0) {
			buffer_index--;
			printf("\b \b");
		}
		return;
	}

	if (buffer_index < max_length - 1) {
		buffer[buffer_index++] = character;
		printf("%c", character);
	}

    buffer[buffer_index] = '\0';
}

void handle_user_input(char* buffer) {
	if (buffer_reset) {
		if (strcmp(buffer, "NEXT") == 0) {
			printf("\r\nAttempting to view next song...\r\n\r\n");
		} else if (strcmp(buffer, "PLAY") == 0) {
			printf("\r\nAttempting to play song...\r\n\r\n");
			LED_On();
		} else if (strcmp(buffer, "PAUSE") == 0) {
			printf("\r\nAttempting to pause song...\r\n\r\n");
		} else if (strcmp(buffer, "STOP") == 0) {
			printf("\r\nAttempting to stop song...\r\n\r\n");
			LED_Off();
		} else if (strcmp(buffer, "HELP") == 0) {
			display_menu();
		} else {
			printf("\r\nInvalid command\r\n\r\n");
		}

		buffer_index = 0;
		buffer_reset = 0;
	}
}

void run_project() {
	display_menu();

	while (1) {
		read_input_string(buffer_input, 7);
		handle_user_input(buffer_input);
	}
}
