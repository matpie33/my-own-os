#include <kernel/buddy_alocator.h>
#include <kernel/virtual_memory_manager.h>
#include <string.h>
#include <kernel/math.h>
#include <kernel/bitmap.h>

free_block_info_t *first_free_region_info;
uint32_t *regions_bitmap;

extern inline uint32_t get_regions_bitmap_size()
{
    return PAGE_SIZE / (8 * sizeof(free_block_info_t) + 1); // page size - size of bitmap = number of structs * size of struct in bytes, number of structs = 8 * size of bitmap in bytes
}

void initialize_buddy_blocks(virtual_address free_addresses_end, virtual_address address_for_first_free_region_info)
{
    regions_bitmap = (uint32_t *)address_for_first_free_region_info;
    uint32_t regions_bitmap_size = get_regions_bitmap_size();
    memset(regions_bitmap, 0, regions_bitmap_size);

    first_free_region_info = (free_block_info_t *)(address_for_first_free_region_info + regions_bitmap_size);
    memset(first_free_region_info, 0, sizeof(free_block_info_t));
    first_free_region_info->address_of_block = 0;
    first_free_region_info->next_block = 0;
    first_free_region_info->previous_block = 0;
    first_free_region_info->size_class = log2(free_addresses_end - VIRTUAL_MEMORY_START);
    bitmap_set_bit(0, regions_bitmap);
}

void free_given_block_info(free_block_info_t *block)
{
    uint32_t index_in_regions_bitmap = (uint32_t)block - ((uint32_t)regions_bitmap + get_regions_bitmap_size());
    index_in_regions_bitmap /= sizeof(free_block_info_t);
    bitmap_unset_bit(index_in_regions_bitmap, regions_bitmap);
}

void *allocate_free_block_info()
{
    uint32_t number_of_blocks = get_regions_bitmap_size() * BITS_PER_BYTE;
    int32_t first_free_block = get_first_free_block_index(regions_bitmap, number_of_blocks);
    printf("b %d,", first_free_block);
    bitmap_set_bit(first_free_block, regions_bitmap);
    if (first_free_block != -1)
    {
        return (void *)((uint32_t)regions_bitmap + get_regions_bitmap_size() + first_free_block * sizeof(free_block_info_t));
    }
    else
    {
        return 0;
    }
}

void *allocate_virtual_block(uint32_t request_size)
{
    free_block_info_t *block = first_free_region_info;
    while (block->next_block && request_size > power(2, block->size_class))
    {
        block = block->next_block;
    };

    uint32_t size_class = block->size_class;

    while (size_class >= MINIMUM_BUDDY_BLOCK_SIZE_CLASS && request_size <= power(2, size_class - 1))
    {
        uint32_t buddy_address = TOGGLE_BIT(block->address_of_block, size_class - 1);
        virtual_address block_info_address = (virtual_address)allocate_free_block_info();
        if (!block_info_address)
        {
            return 0;
        }
        free_block_info_t *buddy_block = (free_block_info_t *)block_info_address;
        memset(buddy_block, 0, sizeof(free_block_info_t));
        buddy_block->address_of_block = buddy_address;
        if (block->next_block)
        {
            block->next_block->previous_block = buddy_block;
        }
        buddy_block->size_class = size_class - 1;
        buddy_block->next_block = block->next_block;
        block->next_block = buddy_block;
        size_class--;
        block->size_class = size_class;
    };
    free_block_info_t *buddy = block->next_block;
    if (block->previous_block)
    {
        block->previous_block->next_block = buddy;
    }
    if (block->next_block)
    {
        block->next_block->previous_block = block->previous_block;
    }
    if (block == first_free_region_info)
    {
        first_free_region_info = buddy;
    }
    free_given_block_info(block);
    return (void *)block->address_of_block + VIRTUAL_MEMORY_START;
}

void create_new_block(virtual_address address, uint32_t size_class, free_block_info_t *block)
{
    free_block_info_t *new_free_block = (free_block_info_t *)allocate_free_block_info();
    if (!new_free_block)
    {
        printf("No more memory");
        return;
    }
    new_free_block->address_of_block = address;
    new_free_block->size_class = size_class;
    new_free_block->next_block = block;
    new_free_block->previous_block = block->previous_block;
    new_free_block->previous_block->next_block = new_free_block;
    block->previous_block = new_free_block;
    if (block == first_free_region_info)
    {
        first_free_region_info = new_free_block;
    }
}

void free_virtual_block(virtual_address address, uint32_t size)
{
    address -= VIRTUAL_MEMORY_START;
    uint32_t size_class_of_freed_block = log2(size - 1) + 1;
    uint32_t buddy_address = TOGGLE_BIT(address, size_class_of_freed_block);
    free_block_info_t *block = first_free_region_info;
    while (block && block->address_of_block < buddy_address)
    {

        block = block->next_block;
    }
    if (!block || block->address_of_block != buddy_address)
    {
        create_new_block(address, size_class_of_freed_block, block);
        return;
    }
    if (block->size_class == size_class_of_freed_block)
    {
        uint32_t size_class = size_class_of_freed_block + 1;
        block->size_class++;
        block->address_of_block = min(address, buddy_address);

        free_block_info_t *adjacent_block = block;
        while (adjacent_block)
        {
            buddy_address = TOGGLE_BIT(block->address_of_block, block->size_class);
            if (buddy_address < block->address_of_block)
            {
                adjacent_block = block->previous_block;
            }
            else
            {
                adjacent_block = block->next_block;
            }

            if (adjacent_block->size_class == size_class && adjacent_block->address_of_block == buddy_address)
            {
                if (buddy_address < block->address_of_block)
                {
                    block->previous_block = adjacent_block->previous_block;
                }
                else
                {
                    block->next_block = adjacent_block->next_block;
                }
                if (adjacent_block == first_free_region_info)
                {
                    first_free_region_info = block;
                }
                free_given_block_info(adjacent_block);
                size_class++;
                block->size_class++;
                block->address_of_block = min(block->address_of_block, adjacent_block->address_of_block);
                adjacent_block = block->next_block;
            }
            else
            {
                return;
            }
        }
    }
}