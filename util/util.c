#include <stdint.h>
#include "../cpu/types.h"

boolean check_if_bit_is_set(uint8_t byte_value, uint8_t bit_0_based){
	return byte_value & 1 << bit_0_based;
}
