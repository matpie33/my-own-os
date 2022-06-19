#include <stdio.h>
#include <kernel/descriptor_tables.h>
#include <kernel/interrupt_tables.h>
#include <kernel/tty.h>
#include <kernel/timer.h>
#include <kernel/interrupt_requests.h>
#include <stdint.h>

void kernel_main(void) {
	terminal_initialize();
	init_descriptor_tables();
	init_idt();
	IRQ_clear_mask(0);
	init_timer(50);
	asm volatile ("int $0x20");
}
