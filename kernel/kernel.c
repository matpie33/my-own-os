#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../libc/strings.h"
#include "../libc/function.h"
#include "../libc/hex_to_string.h"
#include "../graphics/draw_pixel.h"

void start() { 

//	clear_screen();
//	println("Welcome to my os.");

	int i;
	for (i=0; i<700; i++){
		put_pixel(i,2,0x00FFFFFF);
	}

	isr_install();
	asm volatile("sti");
//
//	init_timer(50);
	init_keyboard();
//
//	__asm__ __volatile__("int $2");
//	__asm__ __volatile__("int $3");
}        

void user_input(char *input) {
    if (strcmp(input, "END") == 0) {
        print("Stopping the CPU. Bye!\n");
        __asm__ ("hlt");
    }
    print("You said: ");
    println(input);
}
