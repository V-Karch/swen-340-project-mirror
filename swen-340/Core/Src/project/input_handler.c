#include "input_handler.h"
#include "project.h"
#include "midi_parser.h"
#include <stdio.h>
#include <string.h>

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
        if (strcmp(buffer, "NEXT") == 0) next_song_display();
        else if (strcmp(buffer, "PLAY") == 0) { printf("\r\nPlay song\r\n\r\n"); LED_Status = 1; }
        else if (strcmp(buffer, "PAUSE") == 0) { printf("\r\nPause song\r\n\r\n"); LED_Status = 2; }
        else if (strcmp(buffer, "STOP") == 0) { printf("\r\nStop song\r\n\r\n"); LED_Status = 0; }
        else if (strcmp(buffer, "HELP") == 0) display_menu();

        buffer_index = 0;
        buffer_reset = 0;
        printf(">>> ");
    }
}
