#include <stdint.h>
#include <stdbool.h>
#include <kernel/physical_memory_manager.h>
#include <string.h>
#include <stdio.h>
#include <kernel/common.h>
#include <kernel/math.h>

static uint32_t memory_size_in_kb = 0;
static uint32_t used_blocks = 0;

static uint32_t total_blocks = 0 ;
static uint32_t* memory_bitmap = 0;

static inline void memory_map_set_bit (int bit_number){
    memory_bitmap [bit_number/32] |= (1 << (bit_number%32));
}

static inline void memory_map_unset_bit (int bit_number){
    memory_bitmap [bit_number/32] &= ~(1 << (bit_number%32));
}

static inline bool memory_map_is_bit_set (int bit_number) {
 
 return memory_bitmap[bit_number / 32] &  (1 << (bit_number % 32));
}

static inline bool is_bit_set_in_array_element(uint32_t bit_index, uint32_t array_index){
    return memory_bitmap[array_index] & bit_index;
}

int32_t get_first_free_block_index () {
 
	for (uint32_t i=0; i< total_blocks / BLOCKS_PER_INT; i++)
		if (memory_bitmap[i] != ALL_BITS_IN_INT_ARE_TAKEN){
			for (int32_t j=0; j< BLOCKS_PER_INT; j++) {		
 
				int32_t bit = 1 << j;
				if (! is_bit_set_in_array_element(bit, i) )
					return i*BLOCKS_PER_INT+j;
			}
        }
 
	return -1;
}

int32_t get_multiple_contiguos_free_blocks (uint32_t number_of_blocks) {
 
	int32_t starting_index = -1;
	int32_t last_index_found = -1;
	int32_t blocks_left_to_find = number_of_blocks;
	for (uint32_t i=0; i< total_blocks / BLOCKS_PER_INT; i++)
		if (memory_bitmap[i] != ALL_BITS_IN_INT_ARE_TAKEN){
			for (int32_t j=0; j< BLOCKS_PER_INT; j++) {		
 
				int32_t bit = 1 << j;
				int32_t current_frame = i*BLOCKS_PER_INT+j;
				if (! is_bit_set_in_array_element(bit, i) ){
					if (starting_index == -1 || last_index_found +1 !=current_frame){
						starting_index = current_frame;
						blocks_left_to_find = number_of_blocks;
					}
					blocks_left_to_find--;
					last_index_found = current_frame;
					if (blocks_left_to_find ==0){
						return starting_index;
					}
				}
			}
        }
 
	return -1;
}




void init_free_region (uint64_t base_address, uint64_t size) {
 
	uint64_t block_index = divide_round_up(base_address, BLOCKS_SIZE);
	uint64_t number_of_blocks = divide_round_up(size, BLOCKS_SIZE);
	printf("initializing free region address: %lli, number of blocks: %lli\n",  base_address, number_of_blocks);
 
	for (; number_of_blocks>0; number_of_blocks--) {
		memory_map_unset_bit (block_index++);
		used_blocks--;
	}
 
}


void init_reserved_region (uint32_t base_address, uint32_t size) {
 
	uint32_t block_index = divide_round_up(base_address ,BLOCKS_SIZE);
	uint32_t number_of_blocks = divide_round_up(size ,BLOCKS_SIZE);
 
	for (; number_of_blocks>0; number_of_blocks--) {
		memory_map_set_bit (block_index++);
		used_blocks++;
	}
}

static inline uint32_t get_free_block_count (){
	return total_blocks - used_blocks;
}

void* allocate_block () {
 
	if (get_free_block_count() <= 0)
		return 0;	
 
	int32_t block_index = get_first_free_block_index ();
 
	if (block_index == -1)
		return 0;	
 
	memory_map_set_bit (block_index);
 
	uint32_t addr = block_index * BLOCKS_SIZE;
	used_blocks++;
	printf ("allocated address %d\n", addr);
	return (void*)addr;
}

void* allocate_blocks(uint32_t number_of_blocks) {
 
	if (get_free_block_count() <= 0)
		return 0;	
 
	int32_t block_index = get_multiple_contiguos_free_blocks (number_of_blocks);
 
	if (block_index == -1)
		return 0;	
 
	for (uint32_t i=0; i<number_of_blocks; i++){
		memory_map_set_bit (block_index + i);
		used_blocks++;
	}
 
	uint32_t addr = block_index * BLOCKS_SIZE;
	
	printf ("allocated address %d for n = %d blocks\n", addr, number_of_blocks);
	return (void*)addr;
}

void free_blocks (void* p, uint32_t number_of_blocks) {
 
	uint32_t addr = (uint32_t)p;
	uint32_t block = addr / BLOCKS_SIZE;

	for (uint32_t i=0; i< number_of_blocks; i++){
		memory_map_unset_bit (block);
		used_blocks--;
	}
 
}

void free_block (void* p) {
 
	uint32_t addr = (uint32_t)p;
	uint32_t block = addr / BLOCKS_SIZE;
 
	memory_map_unset_bit (block);
 
	used_blocks--;
}

void initialize (memory_info memory_info) {
 
	memory_size_in_kb	=	memory_info.memory_size;
	total_blocks	=	memory_info.memory_size  / BLOCKS_SIZE;
	used_blocks	=	total_blocks;
	uint32_t number_of_ints_in_bitmap = total_blocks / BLOCKS_PER_INT;
	
	for (uint32_t i=0; i<memory_info.number_of_free_regions; i++){
		memory_region region = memory_info.free_memory_regions[i];
		if (region.length > number_of_ints_in_bitmap){
			if (region.address < GIGABYTE){
				uint32_t address = (uint32_t) region.address;
				memory_bitmap = (uint32_t*)address;
			}
			uint64_t bytes_used_for_bitmap = total_blocks/BLOCKS_PER_BYTE;
			printf("memory bitmap address: %d size %lli\n", (uint32_t)memory_bitmap, bytes_used_for_bitmap);
			region.address += bytes_used_for_bitmap;
			region.length -= bytes_used_for_bitmap;
			memory_info.free_memory_regions[i] = region;
			break;
		}
	}

	memset (memory_bitmap, 0xffffffff, number_of_ints_in_bitmap);

	for (uint32_t i=0; i< memory_info.number_of_free_regions; i++){
		memory_region region = memory_info.free_memory_regions[i];
		init_free_region(region.address, region.length);
		
	}

	printf ("total blocks: %d\n", total_blocks);
	printf ("used blocks: %d\n", used_blocks);
 
}