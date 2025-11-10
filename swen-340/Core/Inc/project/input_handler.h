#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <stdint.h>

void display_menu(void);
void read_input_string(char *buffer, uint32_t max_length);
void handle_user_input(char *buffer);
void next_song_display(void);

#endif
