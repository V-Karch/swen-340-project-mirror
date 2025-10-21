/*
 * project.c
 *
 *  Created on: Jan 8, 2023
 *      Author: V-Karch
 */

#include "project.h"

uint8_t LED_status = 0;

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

uint32_t read_var_len(const uint8_t **ptr) {
    uint32_t value = 0;
    uint8_t c;
    do {
        c = *(*ptr)++;
        value = (value << 7) | (c & 0x7F);
    } while (c & 0x80);
    return value;
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

midi_info parse_midi_meta_events(uint8_t *data, uint32_t size) {
    midi_info info;
    memset(&info, 0, sizeof(info));

    uint8_t *ptr = data;
    uint8_t *data_end = data + size;

    while (ptr < data_end - 8) {
        // Look for MIDI track chunk
        if (memcmp(ptr, "MTrk", 4) == 0) {
            ptr += 4;
            uint32_t track_length = (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
            ptr += 4;

            uint8_t *track_end = ptr + track_length;
            while (ptr < track_end) {
                read_var_len((const uint8_t**)&ptr); // Skip delta-time

                if (*ptr == 0xFF) { // Meta-event
                    ptr++;
                    uint8_t meta_type = *ptr++;
                    uint32_t meta_length = read_var_len((const uint8_t**)&ptr);

                    switch (meta_type) {
                        case 0x02: // Copyright
                            if (info.copyright[0] == 0) {
                                uint32_t copy_len = meta_length < sizeof(info.copyright) - 1
                                                    ? meta_length : sizeof(info.copyright) - 1;
                                // This feels like a crime to write
                                // Memory safety who???????
                                memcpy(info.copyright, ptr, copy_len);
                                info.copyright[copy_len] = '\0';
                            }
                            break;
                        case 0x03: // Title
                            if (info.title[0] == 0) {
                                uint32_t title_len = meta_length < sizeof(info.title) - 1
                                                     ? meta_length : sizeof(info.title) - 1;
                                memcpy(info.title, ptr, title_len);
                                info.title[title_len] = '\0';
                            }
                            break;
                        case 0x51: // Tempo
                            if (meta_length == 3 && info.tempo == 0) {
                                info.tempo = (ptr[0] << 16) | (ptr[1] << 8) | ptr[2];
                            }
                            break;
                        default:
                            break;
                    }

                    ptr += meta_length;
                } else if (*ptr == 0xF0 || *ptr == 0xF7) { // SysEx event
                    ptr++;
                    uint32_t sysex_length = read_var_len((const uint8_t**)&ptr);
                    ptr += sysex_length;
                } else { // MIDI event
                    uint8_t status = *ptr++;
                    uint8_t event_type = status >> 4;

                    // Events with 1 or 2 data bytes
                    if (event_type >= 0x8 && event_type <= 0xE) {
                        ptr += (event_type == 0xC || event_type == 0xD) ? 1 : 2;
                    }
                }
            }
        } else {
            ptr++;
        }
    }

    return info;
}


void handle_LED() {
    switch (LED_status) {
        case 0:
        	LED_Off();
        	break;
        case 1:
			LED_On();
			break;
        case 2:
            LED_Toggle();
            delay_systick();
            break;
    }
}

void handle_user_input(char* buffer) {
	if (buffer_reset) {
		if (strcmp(buffer, "NEXT") == 0) {
			printf("\r\nAttempting to view next song...\r\n\r\n");
			song s = get_song(song_number);

			if (song_number == 4) {
				song_number = 0;
			} else {
				song_number++;
			}

			midi_info info = parse_midi_meta_events(s.p_song, s.size);

			printf("Title: %s\r\n", info.title);
			printf("Copyright: %s\r\n", info.copyright); // Why does this just not display for some tracks
			// Am I losing my mind
			// This function hurt my brain and took ages to make
			printf("Tempo: %u\r\n", (unsigned int)info.tempo);

		} else if (strcmp(buffer, "PLAY") == 0) {
			printf("\r\nAttempting to play song...\r\n\r\n");
			LED_status = 1;
		} else if (strcmp(buffer, "PAUSE") == 0) {
			printf("\r\nAttempting to pause song...\r\n\r\n");
			LED_status = 2;
		} else if (strcmp(buffer, "STOP") == 0) {
			printf("\r\nAttempting to stop song...\r\n\r\n");
			LED_status = 0;
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
		handle_LED();
	}
}
