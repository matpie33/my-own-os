#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../cpu/types.h"
#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../libc/strings.h"
#include "../libc/function.h"
#include "../libc/hex_to_string.h"
#include "../graphics/draw_pixel.h"
#include "../graphics/draw_string.h"

void start() { 

//	clear_screen();
	println("Welcome to my os.");
	char * string = "Hello world, this is my os.";
	draw_string(string,200, 200, 0x00FFFFFF);

	isr_install();
	asm volatile("sti");
//	init_timer(50);
	init_keyboard();
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
