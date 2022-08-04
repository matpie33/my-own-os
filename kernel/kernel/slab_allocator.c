#include <kernel/slab_allocator.h>
#include <kernel/virtual_memory_manager.h>
#include <stdint.h>
#include <kernel/string_utils.h>
#include <kernel/math.h>
#include <string.h>

static cache_sizes_t cache_sizes[] = {
    {32, NULL, NULL},
    {64, NULL, NULL},
    {128, NULL, NULL},
    {256, NULL, NULL},
    {512, NULL, NULL},
    {1024, NULL, NULL},
    {2048, NULL, NULL},
    {4096, NULL, NULL},
    {8192, NULL, NULL},
    {16384, NULL, NULL},
    {32768, NULL, NULL},
    {65536, NULL, NULL},
    {131072, NULL, NULL},
    {0, NULL, NULL}};

static kmem_cache_t cache_of_caches =
    {
        .name = "kmem_cache",
        .flags = SLAB_NO_REAP,
        .object_size = sizeof(kmem_cache_t)};

void initialize_slab_allocator()
{
    void *slabs_address = allocate_pages(1);
    uint32_t objects_per_slab = PAGE_SIZE / (sizeof(uint32_t) + sizeof(kmem_cache_t));
    cache_of_caches.slabs_free = slabs_address;
    cache_of_caches.objects_per_slab = objects_per_slab;
    cache_of_caches.pages_per_slab = 1;

    slab_t *free_slab = (slab_t *)slabs_address;
    free_slab->active_objects_count = 0;
    free_slab->first_free_object_index = 0;
    free_slab->first_object_address = (uint32_t)slabs_address + sizeof(slab_t) + sizeof(uint32_t) * objects_per_slab;

    uint32_t exponent = FIRST_SIZE_CACHE_EXPONENT;
    kmem_cache_t *previous_cache = &cache_of_caches;
    void *page_for_strings = allocate_pages(1);
    char *buffer = (char *)page_for_strings;
    for (uint32_t i = 0; i < SIZES_CACHE_SIZE; i++)
    {
        kmem_cache_t *cache = (kmem_cache_t *)((uint32_t)cache_of_caches.slabs_free->first_object_address + i * sizeof(kmem_cache_t));
        memset(cache, 0, sizeof(kmem_cache_t));
        uint32_t number_of_pages = 1;

        char char_buffer[32];
        uint32_t cache_size = power(2, exponent);
        if (cache_size * 4 > PAGE_SIZE)
        {
            number_of_pages = cache_size * 4 / PAGE_SIZE;
        }

        join(int_to_string(cache_size, char_buffer), "_bytes_cache", buffer);
        cache->name = buffer;
        cache->objects_per_slab = number_of_pages * PAGE_SIZE / (sizeof(uint32_t) + cache_size);
        cache->object_size = cache_size;
        cache->pages_per_slab = number_of_pages;
        void *slab_address = allocate_pages(number_of_pages);
        cache->slabs_free = slab_address;

        slab_t *slab = (slab_t *)slab_address;
        slab->active_objects_count = 0;
        slab->first_free_object_index = 0;
        slab->first_object_address = (uint32_t)slab_address + sizeof(slab_t) + sizeof(uint32_t) * cache->objects_per_slab;

        exponent++;
        previous_cache->next = cache;
        previous_cache = cache;
        buffer += strlen(buffer) + 1;
    }
}
