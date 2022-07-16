#include <stdint.h>
#include <stdbool.h>
#include <kernel/physical_memory_manager.h>
#include <string.h>
#include <stdio.h>
#include <kernel/common.h>
#include <kernel/math.h>
#include <kernel/bitmap.h>

static uint32_t memory_size_in_kb = 0;
static uint32_t used_blocks = 0;

static uint32_t total_blocks = 0;
static uint32_t *memory_bitmap = 0;

void init_free_region(uint64_t base_address, uint64_t size)
{

	uint64_t block_index = divide_round_up(base_address, BLOCK_SIZE);
	uint64_t number_of_blocks = divide_round_up(size, BLOCK_SIZE);
	printf("initializing free region address: %lli, number of blocks: %lli\n", base_address, number_of_blocks);

	for (; number_of_blocks > 0; number_of_blocks--)
	{
		bitmap_unset_bit(block_index++, memory_bitmap);
		used_blocks--;
	}
}

void init_reserved_region(uint32_t base_address, uint32_t size)
{

	uint32_t block_index = divide_round_up(base_address, BLOCK_SIZE);
	uint32_t number_of_blocks = divide_round_up(size, BLOCK_SIZE);

	for (; number_of_blocks > 0; number_of_blocks--)
	{
		bitmap_set_bit(block_index++, memory_bitmap);
		used_blocks++;
	}
}

static inline uint32_t get_free_block_count()
{
	return total_blocks - used_blocks;
}

void *allocate_block()
{

	if (get_free_block_count() <= 0)
		return 0;

	int32_t block_index = get_first_free_block_index(memory_bitmap, total_blocks);

	if (block_index == -1)
		return 0;

	bitmap_set_bit(block_index, memory_bitmap);

	uint32_t addr = block_index * BLOCK_SIZE;
	used_blocks++;
	return (void *)addr;
}

void *allocate_blocks(uint32_t number_of_blocks)
{

	if (get_free_block_count() <= 0)
		return 0;

	int32_t block_index = get_multiple_contiguous_free_blocks(number_of_blocks, total_blocks, memory_bitmap);

	if (block_index == -1)
		return 0;

	for (uint32_t i = 0; i < number_of_blocks; i++)
	{
		bitmap_set_bit(block_index + i, memory_bitmap);
		used_blocks++;
	}

	uint32_t addr = block_index * BLOCK_SIZE;

	printf("allocated address %d for n = %d blocks\n", addr, number_of_blocks);
	return (void *)addr;
}

void free_blocks(void *p, uint32_t number_of_blocks)
{

	uint32_t addr = (uint32_t)p;
	uint32_t block = addr / BLOCK_SIZE;

	for (uint32_t i = 0; i < number_of_blocks; i++)
	{
		bitmap_unset_bit(block, memory_bitmap);
		used_blocks--;
	}
}

void free_block(void *p)
{

	uint32_t addr = (uint32_t)p;
	uint32_t block = addr / BLOCK_SIZE;

	bitmap_unset_bit(block, memory_bitmap);

	used_blocks--;
}

void initialize(memory_info memory_info)
{

	memory_size_in_kb = memory_info.memory_size;
	total_blocks = memory_info.memory_size / BLOCK_SIZE;
	used_blocks = total_blocks;
	uint32_t number_of_ints_in_bitmap = total_blocks / BLOCKS_PER_INT;

	for (uint32_t i = 0; i < memory_info.number_of_free_regions; i++)
	{
		memory_region region = memory_info.free_memory_regions[i];
		if (region.length > number_of_ints_in_bitmap)
		{
			if (region.address < FOUR_GIGABYTE)
			{
				uint32_t address = (uint32_t)region.address;
				memory_bitmap = (uint32_t *)address;
			}
			uint64_t bytes_used_for_bitmap = total_blocks / BLOCKS_PER_BYTE;
			printf("memory bitmap address: %d size %lli\n", (uint32_t)memory_bitmap, bytes_used_for_bitmap);
			region.address += bytes_used_for_bitmap;
			region.length -= bytes_used_for_bitmap;
			memory_info.free_memory_regions[i] = region;
			break;
		}
	}

	memset(memory_bitmap, 0xffffffff, number_of_ints_in_bitmap);

	for (uint32_t i = 0; i < memory_info.number_of_free_regions; i++)
	{
		memory_region region = memory_info.free_memory_regions[i];
		init_free_region(region.address, region.length);
	}

	printf("total blocks: %d\n", total_blocks);
	printf("used blocks: %d\n", used_blocks);
}