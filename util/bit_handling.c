#include "stdint.h"

#include "../cpu/types.h"

boolean is_bit_set(uint32_t byte_value, uint8_t bit_index_0_based) {
	return byte_value & 1 << bit_index_0_based;
}

uint8_t set_bit(uint32_t byte_value, uint8_t bit_index_0_based) {
	byte_value |= 1 << bit_index_0_based;
	return byte_value;
}

uint8_t clear_bit(uint32_t byte_value, uint8_t bit_index_0_based) {
	byte_value &= ~(1 << bit_index_0_based);
	return byte_value;
}
