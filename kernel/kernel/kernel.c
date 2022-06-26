#include <stdio.h>
#include <kernel/global_descriptor_tables.h>
#include <kernel/interrupt_tables.h>
#include <kernel/tty.h>
#include <kernel/timer.h>
#include <kernel/interrupt_requests.h>
#include <stdint.h>
#include <kernel/keyboard.h>
#include <kernel/key_press_handler.h>
#include <kernel/multiboot.h>
#include <kernel/memory_detecting.h>
#include <kernel/physical_memory_manager.h>



extern void enable_interrupts();

void memory_test (){
	uint8_t* memory = allocate_block();
	for (int i=0; i<4096; i++){
		memory[i] = 2138921;
	}
	free_block((void*)memory);
	uint8_t* memory3 = allocate_block();
}

void kernel_main(multiboot_info_t* mbd, uint32_t magic) {
	terminal_initialize();
	init_gdt();
	init_idt();
	IRQ_clear_mask(2);
	memory_info memory_info = detect_memory(mbd, magic);
	initialize(memory_info);
	memory_test();
	init_keyboard();
	enable_interrupts();
	set_display_pressed_keys_on_screen(true);
	// init_timer(50);
	asm volatile ("int $0x03");
	for(;;) asm("hlt");
}

