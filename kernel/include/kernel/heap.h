#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#define INITIAL_PAGES_FOR_HEAP 25

typedef struct free_region_info
{
    uint32_t address;
    uint32_t length;
    struct free_region_info *next_region;

} free_region_info_t;

void set_up_heap();
void *kmalloc(uint32_t number_of_bytes);
void kfree(uint32_t start_address, uint32_t number_of_bytes);
void print_free_regions();

#endif