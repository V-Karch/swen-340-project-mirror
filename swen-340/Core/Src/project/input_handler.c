#include "input_handler.h"
#include "project.h"
#include "midi_parser.h"
#include "tone.h"
#include <stdio.h>
#include <stdlib.h>
/**
 * @file input_handler.c
 * @brief Implements user input processing and command handling for the LED control project.
 *
 * This module manages serial input from the user, interprets commands such as
 * PLAY, PAUSE, STOP, and NEXT, and interfaces with other project modules like
 * LED control and MIDI parsing to perform appropriate actions.
 */

/* External global variables shared with project.c */
extern uint8_t LED_Status;      /**< LED state (0=off, 1=on, 2=flashing) */
extern uint8_t MODE_STATUS;     /**< System mode (0=manual, 1=remote) */
extern uint8_t buffer_reset;    /**< Flag indicating that input buffer should reset */
extern uint32_t buffer_index;   /**< Current index within the input buffer */
extern uint8_t song_number;     /**< Index of the current song */
extern char buffer_input[7];    /**< Input buffer for user commands */

/**
 * @brief Display the command menu over the serial interface.
 *
 * Prints the list of available user commands (NEXT, PLAY, PAUSE, STOP)
 * to assist the user when interacting with the system through UART.
 *
 * @return void
 */
void display_menu() {
    printf("***REMOTE LED CONTROL MENU***\r\n"
           "Available User Commands\r\n"
           "NEXT - Show next song info\r\n"
           "PLAY - Play the song (LED ON)\r\n"
           "PAUSE - Pause the song (LED FLASH)\r\n"
           "STOP - Stop the song (LED off)\r\n\r\n");
}

/**
 * @brief Read user input from the serial interface in a non-blocking manner.
 *
 * Captures user-typed characters from USART2 and stores them into the
 * provided buffer. Handles backspaces, carriage returns, and newlines.
 * When a complete command is entered (terminated by newline or carriage return),
 * sets the @c buffer_reset flag to signal that a command is ready for processing.
 *
 * @param buffer Pointer to the buffer where user input will be stored.
 * @param max_length Maximum number of characters (including null terminator).
 *
 * @return void
 */
void read_input_string(char *buffer, uint32_t max_length) {
    char character = (char)USART_Read_Non_Blocking(USART2);

    if (MODE_STATUS == 0 || character == 0) return;

    if (character == '\n' || character == '\r') {
        buffer_reset = 1;
        printf("\r\n");
        return;
    }

    if (character == 0x08 || character == 0x7F) {  /* Handle backspace */
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

/**
 * @brief Display information about the next song in the playlist.
 *
 * Retrieves the next song using @c get_song(), parses its MIDI metadata,
 * and prints the title, copyright,
 * and tempo information via the serial interface.
 *
 * Automatically cycles back to the first song when the end of the list is reached.
 *
 * @return void
 */
void next_song_display() {
    printf("\r\nAttempting to view next song...\r\n\r\n");
    song s = get_song(song_number);
    song_number = (song_number == 4) ? 0 : song_number + 1;
    midi_info info = parse_midi_meta_events(s.p_song, s.size);

    printf("Title: %s\r\n", info.title);
    printf("Copyright: %s\r\n", info.copyright);
    printf("Tempo: %u\r\n", (unsigned int)info.tempo);
}


void speaker_test() {
	for (int i = 0; i < 128; i++) {
		play_tones(i);
		for (int j = 0; j < 1000; j++) {
			play_freq();
		}
	}
}

void next_song_display_debug() {
    printf("\r\nAttempting to view next song...\r\n\r\n");
    song s = get_song(song_number);
    song_number = (song_number == 4) ? 0 : song_number + 1;
    midi_info info = parse_midi_meta_events(s.p_song, s.size);

    printf("Title: %s\r\n", info.title);
    printf("Copyright: %s\r\n", info.copyright);
    printf("Tempo: %u\r\n", (unsigned int)info.tempo);

    printf("\r\n--- MIDI Events ---\r\n");
    for (uint32_t i = 0; i < info.num_events; i++) {
        midi_event *ev = &info.events[i];
        printf("Event %lu: Delta Time: %lu, Type: ", i, ev->delta_time);

        switch (ev->type) {
            case MIDI_EVENT_META:
                printf("Meta, Meta Type: 0x%02X, Length: %lu, Data: ", ev->meta_type, ev->meta_length);
                for (uint32_t j = 0; j < ev->meta_length; j++) {
                    printf("%c", ev->meta_data[j]); // print as char
                }
                printf("\r\n");
                break;
            case MIDI_EVENT_SYSEX:
                printf("SysEx, Length: %lu\r\n", ev->meta_length);
                break;
            case MIDI_EVENT_STANDARD:
                printf("Standard, Status: 0x%02X, Data: ", ev->status);
                for (uint8_t j = 0; j < 2; j++) {
                    if (ev->data[j] != 0) printf("0x%02X ", ev->data[j]);
                }
                printf("\r\n");
                break;
        }
    }

    // Free allocated memory for events
    free(info.events);
}
/**
 * @brief Process and execute user commands entered through the serial interface.
 *
 * When @c buffer_reset is set, this function compares the entered command string
 * against supported commands ("NEXT", "PLAY", "PAUSE", "STOP", "HELP") and performs
 * the corresponding action. After processing, it resets the input buffer and
 * reprints the command prompt (">>> ").
 *
 * @param buffer Pointer to the buffer containing the user input command string.
 *
 * @return void
 */
void handle_user_input(char* buffer) {
    if (buffer_reset) {
        if (strcmp(buffer, "NEXT") == 0) next_song_display_debug();
        else if (strcmp(buffer, "PLAY") == 0) {
        	printf("\r\nPlay song\r\n\r\n");
        	LED_Status = 1;
//        	song s = get_song(song_number);
//        	midi_info info = parse_midi_meta_events(s.p_song, s.size);
//
//        	play_midi_song(&info, DEFAULT_PPQ);
//        	free(info.events);
        	play_song(song_number);
        	// speaker_test();
        }
        else if (strcmp(buffer, "PAUSE") == 0) { printf("\r\nPause song\r\n\r\n"); LED_Status = 2; }
        else if (strcmp(buffer, "STOP") == 0) { printf("\r\nStop song\r\n\r\n"); LED_Status = 0; }
        else if (strcmp(buffer, "HELP") == 0) display_menu();

        buffer_index = 0;
        buffer_reset = 0;
        printf(">>> ");
    }
}
