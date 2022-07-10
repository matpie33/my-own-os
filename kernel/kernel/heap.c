#include <kernel/virtual_memory_manager.h>
#include <kernel/heap.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <kernel/math.h>

free_region_info_t *first_free_region;

void print_free_regions()
{
    free_region_info_t *free_region_info = first_free_region;
    while (free_region_info)
    {
        printf("free region addr %d length %d\n", free_region_info->address, free_region_info->length);
        free_region_info = free_region_info->next_region;
    }
}
void set_up_heap()
{
    first_free_region = allocate_pages_for_heap(INITIAL_PAGES_FOR_HEAP);
    print_free_regions();
}

uint32_t request_more_pages(uint32_t number_of_bytes)
{
    uint32_t number_of_pages_for_new_region = divide_round_up(number_of_bytes, PAGE_SIZE) + INITIAL_PAGES_FOR_HEAP;
    free_region_info_t *new_region_info = allocate_pages_for_heap(number_of_pages_for_new_region);
    if (!new_region_info)
    {
        return 0;
    }

    free_region_info_t *last_free_region = first_free_region;
    while (last_free_region->next_region)
    {
        last_free_region = last_free_region->next_region;
    }
    last_free_region->next_region = new_region_info;
    return new_region_info->address;
}

void *kmalloc(uint32_t number_of_bytes)
{
    free_region_info_t *free_region_info = first_free_region;
    while (free_region_info)
    {
        if (free_region_info->length > number_of_bytes)
        {
            free_region_info->address += number_of_bytes;
            free_region_info->length -= number_of_bytes;
            printf("after malloc %d bytes\n", number_of_bytes);
            print_free_regions();
            return (void *)free_region_info->address;
        }
        free_region_info = free_region_info->next_region;
    }
    if (!request_more_pages(number_of_bytes))
    {
        return 0;
    }
    return kmalloc(number_of_bytes);
}

bool is_free_region_adjacent_left(free_region_info_t *region, uint32_t start_address)
{
    return region->address + region->length == start_address;
}

bool is_free_region_adjacent_right(free_region_info_t *region, uint32_t start_address, uint32_t length)
{
    return region->address == start_address + length;
}

void kfree(uint32_t start_address, uint32_t number_of_bytes)
{

    free_region_info_t *free_region_info = first_free_region;
    while (free_region_info)
    {
        if (is_free_region_adjacent_right(free_region_info, start_address, number_of_bytes))
        {
            free_region_info->address -= number_of_bytes;
            free_region_info->length += number_of_bytes;
            return;
        }
        if (is_free_region_adjacent_left(free_region_info, start_address) && free_region_info->next_region && is_free_region_adjacent_right(free_region_info->next_region, start_address, number_of_bytes))
        {
            kfree((uint32_t)free_region_info->next_region, sizeof(free_region_info_t));
            free_region_info->length += number_of_bytes + free_region_info->next_region->length;
            free_region_info->next_region = free_region_info->next_region->next_region;
            return;
        }
        if (is_free_region_adjacent_left(free_region_info, start_address))
        {
            free_region_info->length += number_of_bytes;
            return;
        }

        if (!free_region_info->next_region || free_region_info->next_region->address > start_address)
        {
            free_region_info_t *new_region = kmalloc(sizeof(free_region_info_t));
            new_region->address = start_address;
            new_region->length = number_of_bytes;
            if (start_address < free_region_info->address)
            {
                first_free_region = new_region;
                new_region->next_region = free_region_info;
            }
            else
            {
                new_region->next_region = free_region_info->next_region;
                free_region_info->next_region = new_region;
            }
            return;
        }
        free_region_info = free_region_info->next_region;
    }
    printf("after freeing %d bytes from address %d", number_of_bytes, start_address);
    print_free_regions();
}