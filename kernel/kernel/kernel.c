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

extern void enable_interrupts();

void kernel_main(void) {
	terminal_initialize();
	init_gdt();
	init_idt();
	IRQ_clear_mask(2);
	init_keyboard();
	enable_interrupts();
	set_display_pressed_keys_on_screen(true);
	// init_timer(50);
	asm volatile ("int $0x03");
	for(;;) asm("hlt");
}
