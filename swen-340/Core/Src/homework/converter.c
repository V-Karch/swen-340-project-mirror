/*
 * converter.c
 *
 *  Created on: Oct 2, 2025
 *      Author: V-Karch
 */

#include "stdint.h"

uint16_t convert_to_uint16(uint8_t* p_value) {
	return p_value[0] << 8 + p_value[1];
}

uint32_t convert_to_uint24(uint8_t* p_value) {
	return p_value[0] << 16 + p_value[1] << 8 + p_value[2];
}

uint32_t convert_to_uint32(uint8_t* p_value) {
	uint32_t value = 0;
	for (int i = 0; i < 4; i++) {
		value = value << 8 + p_value[i];
	}
}
