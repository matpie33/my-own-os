#ifndef MEMORY_DETECTING
#define MEMORY_DETECTING

#include <stdint.h>
#include <kernel/physical_memory_manager.h>
#include <kernel/multiboot.h>

#define ADDRESS_RESERVED_FOR_NULL_POINTER_EXCEPTIONS 0

#define KERNEL_LOCATION_START 1048576
#define KERNEL_LOCATION_END 13631488

#define KERNEL_VIRTUAL_ADDRESS_START 0xc0000000

memory_info detect_memory (multiboot_info_t* multiboot_info, uint32_t magic);

#endif