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

void check_memory (multiboot_info_t* mbd, unsigned int magic){
	/* Make sure the magic number matches for memory mapping*/
    if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        printf("invalid magic number!");
    }
 
    /* Check bit 6 to see if we have a valid memory map */
    if(!(mbd->flags >> 6 & 0x1)) {
        printf("invalid memory map given by GRUB bootloader");
    }
 
    /* Loop through the memory map and display the values */
    int i;
    for(i = 0; i < mbd->mmap_length; 
        i += sizeof(multiboot_memory_map_t)) 
    {
        multiboot_memory_map_t* mmmt = 
            (multiboot_memory_map_t*) (mbd->mmap_addr + i);
 
        printf("Start Addr: %d Length: %d Size: %d Type: %d",
            mmmt->addr, mmmt->len, mmmt->size, mmmt->type);
 
        if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
            /* 
             * Do something with this memory block!
             * BE WARNED that some of memory shown as availiable is actually 
             * actively being used by the kernel! You'll need to take that
             * into account before writing to memory!
             */
        }
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
