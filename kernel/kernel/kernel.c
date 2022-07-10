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
#include <kernel/virtual_memory_manager.h>
#include <kernel/heap.h>

extern void enable_interrupts();

void memory_test()
{
	uint8_t *memory = allocate_block();
	for (int i = 0; i < 4096; i++)
	{
		memory[i] = (uint8_t)2138921;
	}
	free_block((void *)memory);
	allocate_block();
}

void memory_test_1()
{
	void *first = kmalloc(15);
	void *second = kmalloc(5);
	void *third = kmalloc(5);
	kfree((uint32_t)first, 15);
	printf("after freeing first\n");
	print_free_regions();
	kfree((uint32_t)third, 5);
	printf("after freeing third\n");
	print_free_regions();
	kfree((uint32_t)second, 5);
	printf("after freeing second\n");
	print_free_regions();
}

void memory_test_2()
{
	kmalloc(100000);
	printf("after malloc 100000: ");
	print_free_regions();
	kmalloc(3000);
	printf("after malloc 3000 free regions");
	print_free_regions();
}

void kernel_main(multiboot_info_t *mbd, uint32_t magic)
{
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

	set_up_paging();
	set_up_heap();
	memory_test_2();

	printf("paging done");

	// init_timer(50);
	for (;;)
		asm("hlt");
}
