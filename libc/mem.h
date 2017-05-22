#ifndef MEM_H
#define MEM_H

#include <stdint.h>
#include "../cpu/types.h"

void memory_copy ( uint32_t *source , uint32_t *dest , int no_bytes );
void memory_set (uint8_t *dest, uint8_t val, uint32_t len);
uint32_t malloc (uint32_t required_space, variable_type type);
uint32_t calloc (uint32_t required_space, variable_type type);
void test_allocated_memory(uint32_t address, uint32_t size);

#endif
