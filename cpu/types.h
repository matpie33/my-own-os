#ifndef TYPES_H
#define TYPES_H

typedef enum {false, true} boolean;

#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

#endif
