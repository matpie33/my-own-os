#include "../cpu/types.h"
#include "draw_pixel.h"
#include "font_encoding.h"

unsigned int cursor_position_x = 0;
unsigned int cursor_position_y = 0;



void draw_string(char* a, u32 color){
	unsigned int i=0;
	unsigned int j=0;

	unsigned int k=0;
	while (a[k]!=0){
		if (a[k]=='\n'){
			cursor_position_y +=8;
			cursor_position_x = 0;
			break;
		}
		for (i=0; i<8; i++){
			int row = font8x8_basic[(int)a[k]][i];
			for (j=0; j<8; j++){
				u16 bit = (row & (1 << j));
				if (bit!=0){
					put_pixel((u32)(cursor_position_x + j), (u32)(cursor_position_y + i), color);
				}

			}
		}
		cursor_position_x+=8;
		if (cursor_position_x >= 1200){
			cursor_position_y += 8;
			cursor_position_x = 0;
		}
		k++;
	}
}

void print_string(char *a){
	draw_string(a, 0x00FFFFFF);
}

void println(char *a){
	print_string(a);
	print_string("\n");
}
