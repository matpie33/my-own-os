#include <stdint.h>
#include "../graphics/draw_pixel.h"
#include "../libc/hex_to_string.h"
#include "../libc/mem.h"
#include "../graphics/draw_string.h"
#include "../libc/function.h"
#include "../libc/strings.h"
#include "../libc/mem.h"

uint32_t* back_buffer;

void initialize_back_buffer(){
	uint32_t numberOfBytes = best_video_mode.height*best_video_mode.width*best_video_mode.bpp/8;
	back_buffer = (uint32_t *) malloc(numberOfBytes, uint32);

	UNUSED(numberOfBytes);
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



