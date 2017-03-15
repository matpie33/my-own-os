#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../libc/strings.h"

void start() { 

	//TODO fill the whole screen and check if it clears it all
clear_screen();
println("Successfully landed in 32 bit mode.");
println("Now you can type anything to start the OS.");

//	println("hello into my os");
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
