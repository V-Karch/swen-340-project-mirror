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
#include "string.h"
#include "song.h"
#include "systick.h"
#include "LED.h"

typedef struct {
	/* <chunk type><length><format><number of tracks><division>
	 *
	 * <chunk type> -> first four ASCII characters
	 * <length> -> always the value 6 represented as a 32 bit number
	 * <format> -> 16 bit word, values 0, 1, or 2
	 * 0 -> the file contains a single multi-channel track
	 * 1 -> the file contains one or more simultaneous tracks (or MIDI outputs) of a sequence
	 * 2 -> the file contains one or more sequentially independent single-track patterns
	 *
	 * <number of tracks> -> number of tracks in the file, 16 bit word
	 * <division> -> 16 bit word, some complex stuff, refer to MIDI documentation
	 */
	char chunk_type[4];
	unsigned int length;
	unsigned short format;
	unsigned short number_of_tracks;
	unsigned short division;
} midi_header;

typedef struct {
    char title[128];
    char copyright[128];
    uint32_t tempo;  // microseconds per quarter note
} midi_info;

typedef struct {
	uint32_t last_push_time;
	uint8_t is_pressed;
	uint32_t double_press_timeout;
	uint8_t activate_single_press;
} button;

void run_project();
void display_menu();
void read_input_string(char *buffer, uint32_t max_length);
void handle_user_input(char* buffer);
void handle_LED();
uint16_t convert_to_uint16(uint8_t* byte_0);
uint32_t convert_to_uint32(uint8_t* byte_0);
uint32_t read_var_len(const uint8_t **ptr);
midi_info parse_midi_meta_events(uint8_t *data, uint32_t size);

#endif /* INC_PROJECT_H_ */
