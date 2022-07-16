#ifndef BUDDY_ALOCATOR_H
#define BUDDY_ALOCATOR_H

#include <kernel/virtual_memory_manager.h>
#include <stdint.h>
#include <stdbool.h>
#include <kernel/bitmap.h>

#define MINIMUM_BUDDY_BLOCK_SIZE_CLASS 12
#define SIZE_OF_BLOCK_REGIONS_INFO_BITMAP ((PAGE_SIZE) / (BLOCKS_PER_INT))

typedef struct free_block_info
{
    virtual_address address_of_block;
    uint32_t size_class; // exponent of power of 2
    struct free_block_info *previous_block;
    struct free_block_info *next_block;

} free_block_info_t;

void initialize_buddy_blocks(virtual_address free_addresses_end, virtual_address address_for_first_buddy_region_info);
void *allocate_virtual_block(uint32_t request_size);
void free_virtual_block(virtual_address address, uint32_t size);

#endif