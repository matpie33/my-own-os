#include "util.h"
#include "../cpu/isr.h"
#include "../drivers/screen.h"

void start() { 
	clear_screen();
	isr_install();
	
	__asm__ __volatile__("int $2");
	__asm__ __volatile__("int $3");
}        

