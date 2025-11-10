#include "midi_parser.h"
#include <string.h>
#include <stdio.h>

/**
 * @file midi_parser.c
 * @brief Implements MIDI data parsing utilities for extracting metadata from MIDI files.
 *
 * This module provides helper functions for reading multi-byte values,
 * decoding variable-length quantities, and parsing MIDI meta-events
 * such as title, copyright, and tempo.
 */

/**
 * @brief Convert two consecutive bytes into a 16-bit unsigned integer.
 *
 * This function takes a pointer to the first of two bytes stored in big-endian
 * order and converts them into a 16-bit integer value.
 *
 * @param byte_0 Pointer to the first byte of the two-byte sequence.
 * @return The resulting 16-bit unsigned integer.
 */
uint16_t convert_to_uint16(uint8_t* byte_0) {
    return ((*byte_0 << 8) + byte_0[1]);
}

/**
 * @brief Convert four consecutive bytes into a 32-bit unsigned integer.
 *
 * This function takes a pointer to the first of four bytes stored in big-endian
 * order and converts them into a 32-bit integer value.
 *
 * @param byte_0 Pointer to the first byte of the four-byte sequence.
 * @return The resulting 32-bit unsigned integer.
 */
uint32_t convert_to_uint32(uint8_t* byte_0) {
    return ((*byte_0 << 24) + (byte_0[1] << 16) + (byte_0[2] << 8) + (byte_0[3]));
}

/**
 * @brief Read a variable-length quantity from a MIDI data stream.
 *
 * MIDI uses a variable-length encoding scheme for some numeric values.
 * Each byte has its most significant bit (MSB) as a continuation flag:
 * - MSB = 1 → another byte follows
 * - MSB = 0 → this is the last byte in the sequence
 *
 * The function advances the provided pointer as it reads the encoded value.
 *
 * @param ptr Double pointer to the current read position in the MIDI data stream.
 *             The pointer is advanced to the byte following the encoded value.
 *
 * @return The decoded 32-bit unsigned integer value.
 */
uint32_t read_var_len(const uint8_t **ptr) {
    uint32_t value = 0;
    uint8_t c;
    do {
        c = *(*ptr)++;
        value = (value << 7) | (c & 0x7F);
    } while (c & 0x80);
    return value;
}

/**
 * @brief Parse MIDI meta-events and extract key metadata.
 *
 * This function scans raw MIDI file data to locate and parse "MTrk" chunks
 * containing meta-events. It extracts useful metadata such as:
 * - Track title (Meta Type 0x03)
 * - Copyright (Meta Type 0x02)
 * - Tempo (Meta Type 0x51)
 *
 * All data is stored in a @ref midi_info structure, which is returned by value.
 * Unrecognized events and system-exclusive (SysEx) messages are skipped.
 *
 * @param data Pointer to the MIDI data buffer.
 * @param size Size of the MIDI data buffer in bytes.
 * @return A populated @ref midi_info structure containing parsed metadata fields.
 */
midi_info parse_midi_meta_events(uint8_t *data, uint32_t size) {
    midi_info info;
    memset(&info, 0, sizeof(info));
    uint8_t *ptr = data;
    uint8_t *data_end = data + size;

    while (ptr < data_end - 8) {
        /* Locate a MIDI track chunk by its header "MTrk" */
        if (memcmp(ptr, "MTrk", 4) == 0) {
            ptr += 4;

            /* Read the track chunk length (4 bytes, big-endian) */
            uint32_t track_length = (ptr[0] << 24) | (ptr[1] << 16) |
                                    (ptr[2] << 8) | ptr[3];
            ptr += 4;
            uint8_t *track_end = ptr + track_length;

            /* Parse all events within the track */
            while (ptr < track_end) {
                read_var_len((const uint8_t**)&ptr);  /* Skip delta-time */

                if (*ptr == 0xFF) {  /* Meta-event */
                    ptr++;
                    uint8_t meta_type = *ptr++;
                    uint32_t meta_length = read_var_len((const uint8_t**)&ptr);

                    switch (meta_type) {
                        case 0x02:  /**< Copyright notice */
                            if (info.copyright[0] == 0) {
                                uint32_t len = meta_length < sizeof(info.copyright) - 1 ?
                                               meta_length : sizeof(info.copyright) - 1;
                                memcpy(info.copyright, ptr, len);
                                info.copyright[len] = '\0';
                            }
                            break;

                        case 0x03:  /**< Track title */
                            if (info.title[0] == 0) {
                                uint32_t len = meta_length < sizeof(info.title) - 1 ?
                                               meta_length : sizeof(info.title) - 1;
                                memcpy(info.title, ptr, len);
                                info.title[len] = '\0';
                            }
                            break;

                        case 0x51:  /**< Tempo (3-byte value, microseconds per quarter note) */
                            if (meta_length == 3 && info.tempo == 0) {
                                info.tempo = (ptr[0] << 16) | (ptr[1] << 8) | ptr[2];
                            }
                            break;

                        default:
                            /* Ignore other meta types */
                            break;
                    }
                    ptr += meta_length;
                } 
                else if (*ptr == 0xF0 || *ptr == 0xF7) {  /* SysEx event */
                    ptr++;
                    uint32_t len = read_var_len((const uint8_t**)&ptr);
                    ptr += len;
                } 
                else {  /* Standard MIDI event (skip over data bytes) */
                    uint8_t status = *ptr++;
                    uint8_t event_type = status >> 4;
                    ptr += (event_type == 0xC || event_type == 0xD) ? 1 : 2;
                }
            }
        } else {
            /* Skip bytes until "MTrk" is found */
            ptr++;
        }
    }
    return info;
}
