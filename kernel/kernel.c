#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../cpu/types.h"
#include "../drivers/keyboard.h"
#include "../libc/strings.h"
#include "../libc/function.h"
#include "../graphics/draw_pixel.h"
#include "../graphics/draw_string.h"

void start() {
	// Initialization has to be completed first, otherwise keyboard buffer will be full and we won't get
	// characters on the screen

	isr_install();
	asm volatile("sti");
	init_timer(50);
	init_keyboard();
	print_cursor();

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
