#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../cpu/types.h"
#include "../drivers/keyboard.h"
#include "../libc/strings.h"
#include "../libc/function.h"
#include "../libc/hex_to_string.h"
#include "../graphics/draw_pixel.h"
#include "../graphics/draw_string.h"

void start() {
	// Initialization has to be completed first, otherwise keyboard buffer will be full and we won't get
	// characters on the screen

	isr_install();
	asm volatile("sti");
	init_timer(50);
	init_keyboard();
	asm volatile ("int $15");

	print_string("11111111111111111111111111111111111111111111111111111111111111111111111111111111111111");
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
