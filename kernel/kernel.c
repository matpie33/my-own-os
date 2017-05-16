#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../cpu/types.h"
#include "../drivers/keyboard.h"
#include "../libc/strings.h"
#include "../libc/function.h"
#include "../libc/hex_to_string.h"
#include "../graphics/draw_pixel.h"
#include "../graphics/draw_string.h"
#include "../graphics/mouse_cursor.h"
#include "../graphics/back_buffer.h"
#include "../libc/mem.h"

void start() {
	// Initialization has to be completed first, otherwise keyboard buffer will be full and we won't get
	// characters on the screen

	isr_install();
	asm volatile("sti");
	init_timer(50);
	init_keyboard();
	initialize_back_buffer();

	uint32_t newAddress = malloc(10);
//	print_string(hex_to_string(newAddress));
	uint32_t* p = (uint32_t*) newAddress;
//	test_allocated_memory(newAddress, 10);
	int i;
	for (i=0; i<10; i++){
		*(p+i)=0x1+i;
	}
	print_pointer(p, 10);
	println("after copy");
	uint32_t add = malloc(30*sizeof(uint32_t));
	uint32_t* p2 = (uint32_t*) add;
//	test_allocated_memory(add, 30);
	memory_copy(p, p2+10, 10);
	print_pointer(p2, 30);

	println("tests:");
	print_hex(0x1a);
	print_hex(0xff);
	print_hex(0x123);
	print_hex(0xbaba);
	print_hex(0xbacab);
	print_hex(0xbbabba);
	print_hex(0x1234567);
	print_hex(0xa1b2c2c3);
	print_hex(0x00aa00);
	print_hex(0x00001);
	print_hex(0x10000);
	print_hex(0x0000100);

	//TODO catch overflows

//	point center = get_center_of_screen_for_object(16, 16);
//	initialize_cursor(center.x, center.y);


//	int i;
////	//TODO implement double buffering
//	for (i=0; i<1200; i++){
//		put_pixel(i, 100, 0x00FF0000);
//	}
//	for (i=1; i<1400; i++){
//		move_cursor_down(1);
//		sleep(20);
//	}


//	print_string("This is a cool OS.");
}

void user_input(char *input) {
    if (strcmp(input, "END") == 0) {
    	print_string("Stopping the CPU. Bye!\n");
        __asm__ ("hlt");
    }
    print_string("You said: ");
    print_string(input);
    println("");
}
