#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"
#include "../drivers/mouse.h"
#include "../graphics/draw_pixel.h"
#include "../graphics/draw_string.h"
#include "../graphics/mouse_cursor.h"
#include "../libc/printf.h"
#include "../libc/strings.h"

void start() {
	// Initialization has to be completed first, otherwise keyboard buffer will be full and we won't get
	// characters on the screen

	isr_install();
	asm volatile("sti");
	init_timer(50);
	init_keyboard();
	init_mouse();
	initialize_graphics();

	//TODO maybe mouse scaling will help me? or check mouse.c functions and optimize if possible
	//TODO handle the screen properly i.e. use all the screen in bochs

	fill_rectangle(1, 1, 0x00FF0000, 1280, 760);
	fill_rectangle(100, 100, 0x00AAAAAA, 700, 760);
	fill_rectangle(500, 500, 0x00CCCCCC, 100, 100);

	initialize_cursor();

	//	printf("start");

	//TODO filling whole screen is too slow with back buffer
	//	fill_rectangle(1,1, 0x00FF0000, 1280, 760);
	//TODO catch overflows

	//		sleep(4000);
	move_cursor_vertically(5);
	//		sleep(4000);
	//		move_cursor_vertically(5);
	//		sleep(15);
	//	}

}

void user_input(char *input) {
	if (strcmp(input, "END") == 0) {
		println("Stopping the CPU. Bye!");
		__asm__ ("hlt");
	}
	print_string("You said: ");
	print_string(input);
	println("");
}
