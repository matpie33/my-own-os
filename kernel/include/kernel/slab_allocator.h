#ifndef SLAB_ALLOCATOR
#define SLAB_ALLOCATOR

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define SLAB_NO_REAP 1
#define SIZES_CACHE_SIZE 17 - 5 + 1
#define FIRST_SIZE_CACHE_EXPONENT 5

typedef struct slab_s
{
    struct slab_s *list_to_where_it_belongs;
    void *first_object_address;
    unsigned int active_objects_count;
    uint32_t first_free_object_index;
} slab_t;

typedef struct kmem_cache_s
{
    slab_t *slabs_full;
    slab_t *slabs_partial;
    slab_t *slabs_free;
    unsigned int object_size;
    unsigned int flags;
    unsigned int objects_per_slab;
    unsigned int pages_per_slab;

    bool growing;
    unsigned int dflags;

    void (*ctor)(void *, struct kmem_cache_s *, unsigned long);
    void (*dtor)(void *, struct kmem_cache_s *, unsigned long);

    char *name;
    struct kmem_cache_s *next;

} kmem_cache_t;

typedef struct cache_sizes
{
    size_t cs_size;
    kmem_cache_t *cs_cachep;
    kmem_cache_t *cs_dmacachep;
} cache_sizes_t;

void initialize_slab_allocator();

#endif