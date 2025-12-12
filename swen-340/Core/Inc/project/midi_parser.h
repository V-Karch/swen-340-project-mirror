#ifndef MIDI_PARSER_H
#define MIDI_PARSER_H

#define MAX_EVENT_DATA 2
#define MAX_META_DATA 256
#define DEFAULT_PPQ 480

#include <stdint.h>

typedef enum {
    MIDI_EVENT_META,
    MIDI_EVENT_SYSEX,
    MIDI_EVENT_STANDARD
} midi_event_type;

typedef struct {
    uint32_t delta_time;
    midi_event_type type;
    uint8_t status;
    uint8_t data[MAX_EVENT_DATA];     // 1-2 data bytes for standard MIDI
    uint8_t meta_type;                // For meta events
    uint8_t meta_data[MAX_META_DATA]; // For meta events
    uint32_t meta_length;             // Length of meta_data
} midi_event;

typedef struct {
    char title[128];
    char copyright[128];
    uint32_t tempo;
    uint32_t num_events;
    midi_event *events;  // dynamically allocated array of events
} midi_info;

uint16_t convert_to_uint16(uint8_t* byte_0);
uint32_t convert_to_uint32(uint8_t* byte_0);
uint32_t read_var_len(const uint8_t **ptr);
midi_info parse_midi_meta_events(uint8_t *data, uint32_t size);
void play_song(uint8_t number);

#endif
