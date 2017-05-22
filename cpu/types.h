#ifndef TYPES_H
#define TYPES_H

typedef enum {false, true} boolean;
typedef enum {
	uint8, uint16, uint32
} variable_type;

#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

#endif
