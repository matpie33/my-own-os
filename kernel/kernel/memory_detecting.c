#include <kernel/multiboot.h>
#include <stdbool.h>
#include <stdint.h>
#include <kernel/physical_memory_manager.h>
#include <stdio.h>
#include <kernel/memory_detecting.h>
#include <kernel/common.h>

bool is_bit6_set_in_flags_variable (multiboot_info_t* mbd){
	return mbd->flags >> 6 & 0x1;
}

memory_info detect_memory (multiboot_info_t* multiboot_info, uint32_t magic){
    if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        printf("invalid magic number!");
    }
 
    if(!is_bit6_set_in_flags_variable(multiboot_info)) {
        printf("invalid memory map given by GRUB bootloader");
    }

	uint64_t memory_size = 0;
	memory_info memory_info;
	uint32_t number_of_regions = multiboot_info->mmap_length / sizeof(multiboot_memory_map_t);
	
 
    bool is_memory_for_free_regions_reserved = false;
	uint32_t j = 0;
    uint32_t bytes_for_memory_regions_array = sizeof(memory_region) * number_of_regions;
    for(uint32_t i = 0; i < multiboot_info->mmap_length; 
        i += sizeof(multiboot_memory_map_t)){
		multiboot_memory_map_t* memory_map_pointer = 
			(multiboot_memory_map_t*) (multiboot_info->mmap_addr + i);
			
		uint64_t address =  memory_map_pointer->addr;
        uint64_t length = memory_map_pointer->len;

		
		if (address > memory_size){
			memory_size = memory_map_pointer -> addr + length;
		}
        if (memory_map_pointer->type == MULTIBOOT_MEMORY_AVAILABLE) {
            if (address == ADDRESS_RESERVED_FOR_NULL_POINTER_EXCEPTIONS){
                address++;
                length--;
            }
            if (address == KERNEL_LOCATION_START && address + length > KERNEL_LOCATION_END){
                address = KERNEL_LOCATION_END;
                length -= KERNEL_LOCATION_END - KERNEL_LOCATION_START +1;
            }
            if (!is_memory_for_free_regions_reserved && length > bytes_for_memory_regions_array && address < FOUR_GIGABYTE){
                uint32_t address_casted = (uint32_t) address;
                memory_region* memory_regions = (memory_region*)address_casted;
                printf("memory regions array is in: %d\n", memory_regions);
                memory_info.free_memory_regions = memory_regions;
                address += bytes_for_memory_regions_array;
                length -= bytes_for_memory_regions_array;
                is_memory_for_free_regions_reserved = true;
            }
            memory_info.free_memory_regions[j].address = address;
		    memory_info.free_memory_regions[j].length = length;
            j++;
        }
		printf("Start Addr: %lli Length: %lli Size: %d Type: %d\n",
			address, length, memory_map_pointer->size, memory_map_pointer->type);

    }
    memory_info.number_of_free_regions = j;
	memory_info.memory_size = memory_size;
	printf("memory size: %lli\n", memory_size);

    for (uint32_t i=0; i< memory_info.number_of_free_regions; i++){
        printf ("free region: addr %lli length %lli\n", memory_info.free_memory_regions[i].address, memory_info.free_memory_regions[i].length);
    }
    return memory_info;
}