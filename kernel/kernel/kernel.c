#include <stdio.h>
#include <kernel/global_descriptor_tables.h>
#include <kernel/interrupt_tables.h>
#include <kernel/tty.h>
#include <kernel/timer.h>
#include <kernel/interrupt_requests.h>
#include <stdint.h>
#include <kernel/keyboard.h>
#include <kernel/key_press_handler.h>
#include <stdbool.h>
#include <kernel/multiboot.h>

extern void enable_interrupts();

bool is_bit6_set_in_flags_variable (multiboot_info_t* mbd){
	return mbd->flags >> 6 & 0x1;
}

void check_memory (multiboot_info_t* multiboot_info, unsigned int magic){
    if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        printf("invalid magic number!");
    }
 
    if(!is_bit6_set_in_flags_variable(multiboot_info)) {
        printf("invalid memory map given by GRUB bootloader");
    }
 
    int i;
    for(i = 0; i < multiboot_info->mmap_length; 
        i += sizeof(multiboot_memory_map_t)) 
    {
	multiboot_memory_map_t* memory_map_pointer = 
		(multiboot_memory_map_t*) (multiboot_info->mmap_addr + i);
		
	
	printf("Start Addr: %lli Length: %lli Size: %d Type: %d\n",
		(long long) memory_map_pointer ->addr_low | (memory_map_pointer ->addr_high<<32), (long long)memory_map_pointer->len_low | (memory_map_pointer ->len_high <<32), memory_map_pointer->size, memory_map_pointer->type);

    }
}

void kernel_main(multiboot_info_t* mbd, unsigned int magic) {
	terminal_initialize();
	init_gdt();
	init_idt();
	IRQ_clear_mask(2);
	init_keyboard();
	enable_interrupts();
	set_display_pressed_keys_on_screen(true);
	check_memory (mbd, magic);
	// init_timer(50);
	asm volatile ("int $0x03");
	for(;;) asm("hlt");
}
