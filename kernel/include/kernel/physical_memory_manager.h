#ifndef PHYSICAL_MEMORY_MANAGER
#define PHYSICAL_MEMORY_MANAGER

#include <stdint.h>

#define BLOCKS_PER_BYTE 8
#define BLOCKS_SIZE 4096
#define BLOCK_ALIGNMENT BLOCK_SIZE

#define ALL_BITS_IN_INT_ARE_TAKEN 0xFFFFFFFF
#define BLOCKS_PER_INT 32

typedef struct {
    uint64_t address;
    uint64_t length;
} memory_region;

typedef struct {

    uint64_t memory_size;
    memory_region* free_memory_regions;
    uint32_t number_of_free_regions;
	
} memory_info;

void initialize (memory_info memory_info);

void* allocate_block ();
void free_block (void* p) ;
void* allocate_blocks(uint32_t number_of_blocks);
void free_blocks (void* p, uint32_t number_of_blocks);


#endif