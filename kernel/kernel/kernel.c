#include <stdio.h>
#include <kernel/descriptor_tables.h>
#include <kernel/interrupt_tables.h>
#include <kernel/tty.h>

void kernel_main(void) {
	terminal_initialize();
	init_descriptor_tables();
	init_idt();
	asm volatile ("int $0x8");
}
