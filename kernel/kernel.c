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
//	print_string("Initializing back buffer...");
	initialize_back_buffer();

	print_string("Done :)");
	uint32_t newAddress = malloc(1000);
//	print_string(hex_to_string(newAddress));
	uint32_t* pointer = (uint32_t*) newAddress;
	*(pointer+10)=0xdead;

	test_allocated_memory(newAddress,1000);

	uint32_t newAddress2 = malloc(1000);
	test_allocated_memory(newAddress2,1000);

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
