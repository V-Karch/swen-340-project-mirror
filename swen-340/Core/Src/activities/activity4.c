// Create a struct to represent the midi header chunk

#include "song.h"
#include "printf.h"
#include "stdint.h"

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

void run_activity_4() {
	midi_header* sample_header =  (midi_header*)(get_song(0).p_song);
	printf("Chunk Type: %s\r\n", sample_header->chunk_type);
	printf("Length: %ld\r\n", sample_header->length);
	printf("Format: %d\r\n", convert_to_uint16((uint8_t*)(&sample_header->format)));
	printf("Number of tracks: %d\r\n", convert_to_uint16((uint8_t*)&sample_header->number_of_tracks));
	printf("Division: %d\r\n", convert_to_uint16((uint8_t*)&sample_header->division));

}
