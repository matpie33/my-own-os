#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>
#include <stdbool.h>

#define ALL_BITS_IN_INT_ARE_TAKEN 0xFFFFFFFF
#define BLOCKS_PER_INT 32
#define BITS_PER_BYTE 8

typedef struct regions_bitmap_info
{
    uint32_t *bitmap;
    struct regions_bitmap_info *next_bitmap;

} regions_bitmap_info_t;

void bitmap_set_bit(int bit_number, uint32_t *bitmap);
void bitmap_unset_bit(int bit_number, uint32_t *bitmap);
bool bitmap_is_bit_set(int bit_number, uint32_t *bitmap);
int32_t get_first_free_block_index(uint32_t *memory_bitmap, uint32_t total_blocks);
int32_t get_multiple_contiguous_free_blocks(uint32_t number_of_blocks, uint32_t total_blocks, uint32_t *bitmap);

#endif