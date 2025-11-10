#ifndef MIDI_PARSER_H
#define MIDI_PARSER_H

#include <stdint.h>
#include "project.h"  // for midi_info struct

uint16_t convert_to_uint16(uint8_t* byte_0);
uint32_t convert_to_uint32(uint8_t* byte_0);
uint32_t read_var_len(const uint8_t **ptr);
midi_info parse_midi_meta_events(uint8_t *data, uint32_t size);

#endif
