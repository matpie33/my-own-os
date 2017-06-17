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
#include "../libc/mem.h"
#include "../libc/printf.h"
#include "../cpu/types.h"
#include "../drivers/mouse.h"

void start() {
	// Initialization has to be completed first, otherwise keyboard buffer will be full and we won't get
	// characters on the screen

	isr_install();
	asm volatile("sti");
	init_timer(50);
//	init_keyboard(); //TODO mouse doesn't work when keyboard is enabled
	initialize();
//	print_string("dsfajfsdklf"); //TODO what happened to print_string? not working with put_pixel
								 // i.e. with back-buffered version of printing
//	printf("yo");

	initialize_back_buffer();

	init_mouse();

	//TODO filling whole screen is too slow with back buffer
//	fill_rectangle(1,1, 0x00FF0000, 1280, 760);
//	repaint();
	//TODO catch overflows
//	point center = get_center_of_screen_for_object(16, 16);
//	initialize_cursor(center.x, center.y);
//
//	int i;
//	for (i=1; i<1400; i++){
//		move_cursor_down(1);
//
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
