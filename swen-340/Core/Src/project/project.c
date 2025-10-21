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
uint8_t song_number = 0;

uint16_t convert_to_uint16(uint8_t* byte_0) {
	return ((*byte_0 << 8) + byte_0[1]);
}

uint32_t convert_to_uint32(uint8_t* byte_0) {
	return ((*byte_0 << 24) + (byte_0[1] << 16) + (byte_0[2] << 8) + (byte_0[3]));
}

void display_menu() {
	printf("***REMOTE LED CONTROL MENU***\r\nAvailable User Commands\r\nNEXT - Show next song info\r\nPLAY - Play the song (LED ON)\r\nPAUSE - Pause the song (LED FLASH)\r\nSTOP - Stop the song(LED off)\r\n\r\n");
}

void read_input_string(char *buffer, uint32_t max_length) {
    char character = 0;

	character = (char)USART_Read_Non_Blocking(USART2);

	if (character == 0) {
		return;
	}

	if (character == '\n' || character == '\r') {
		buffer_reset = 1;
		printf("\r\n");
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
			midi_header* song_header =  (midi_header*)(get_song(0).p_song);
			printf("Chunk Type: %s\r\n", song_header->chunk_type);
			printf("Length: %ld\r\n", convert_to_uint32((uint8_t*)(&song_header->length)));
			printf("Format: %d\r\n", convert_to_uint16((uint8_t*)(&song_header->format)));
			printf("Number of tracks: %d\r\n", convert_to_uint16((uint8_t*)&song_header->number_of_tracks));
			printf("Division: %d\r\n", convert_to_uint16((uint8_t*)&song_header->division));
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
			// printf("\r\nInvalid command\r\n\r\n");
		}

		buffer_index = 0;
		buffer_reset = 0;

		printf(">>> ");
	}
}

void run_project() {
	display_menu();
	printf(">>> ");

	while (1) {
		read_input_string(buffer_input, 7);
		handle_user_input(buffer_input);
	}
}
