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
#include "../libc/printf.h"

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
	println("p pointer:");
	print_pointer(p,10);
	printf("Your name is %s, you are %d years old, you have a %c as first character"
			"of last name", "mateusz", 23, 'p');

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
