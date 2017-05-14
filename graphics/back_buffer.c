#include <stdint.h>
#include "../graphics/draw_pixel.h"
#include "../libc/hex_to_string.h"
#include "../libc/mem.h"
#include "../graphics/draw_string.h"
#include "../libc/function.h"
#include "../libc/strings.h"

uint32_t* back_buffer;

void initialize_back_buffer(){
	uint32_t numberOfBytes = best_video_mode.height*best_video_mode.width*best_video_mode.bpp/8;
//	char* c = hex_to_string (numberOfBytes);
//	print_string(c);
//	print_string("we are here");

	static uint32_t values[10000000];

//	print_string(hex_to_string(&values));
//	print_string(", ");
//	print_string(hex_to_string((u32)(values+numberOfBytes)));

	uint32_t i=0;
//	if (i<365){
//		print_string("test");
//	}
	for (i=0; i<10000000; i++){
////		char asc[20];
////		int_to_ascii(i, asc);
////		print_string(asc);
////		print_string(",");
//		print_string("abc");
//		values[i] = 1;
	}
//	values[numberOfBytes-1]=0;
//	asm volatile("cli");
//			asm volatile ("hlt");

	UNUSED(values);
	UNUSED(numberOfBytes);
//	back_buffer = values;
}

void write_to_back_buffer(uint32_t* values, int startPosition, int length){
//	memory_copy(values, back_buffer, 3);
//	back_buffer[0]=values[0];
	int i =0;
	print_string("back buffer check");
//	for (i=0; i<length; i++){
//		*(back_buffer+startPosition+i) = *(values+i);
//		char * hex = hex_to_string(*(back_buffer+startPosition+i));
//		print_string(hex);
//	}


	UNUSED(values);
	UNUSED(startPosition);
	UNUSED(length);
	UNUSED(i);
}



