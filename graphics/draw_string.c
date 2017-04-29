#include "../cpu/types.h"
#include "draw_pixel.h"
#include "font_encoding.h"

unsigned int cursor_position_x = 0;
unsigned int cursor_position_y = 0;
#define MARGIN_LEFT (unsigned int) 10
#define MARGIN_UP 10
#define PADDING_WITHIN_LINES 5


const short cursor [16] = {
		0xf000,
		0x9e00,
		0x83c0,
		0x80c0,
		0x8100,
		0x8200,
		0x9100,
		0xa880,
		0xc440,
		0x8220,
		0x0110,
		0x0088,
		0x0044,
		0x0022,
		0x0014,
		0x0008


};

void move_cursor_to_next_line(){
	cursor_position_y +=LETTER_SIZE+PADDING_WITHIN_LINES;
	cursor_position_x = 0;
}

void draw_string(char* a, u32 color){
	unsigned int i=0;
	unsigned int j=0;

	unsigned int k=0;
	while (a[k]!=0){
		if (a[k]=='\n'){
			move_cursor_to_next_line();
			break;
		}
		for (i=0; i<LETTER_SIZE; i++){
			int row = font8x8_basic[(int)a[k]][i];
			for (j=0; j<LETTER_SIZE; j++){
				u16 bit = (row & (1 << j));
				if (bit!=0){
					put_pixel((u32)(MARGIN_LEFT+cursor_position_x + j), (u32)(MARGIN_UP+cursor_position_y
							+ i), color);
				}

			}
		}
		cursor_position_x+=LETTER_SIZE;
		if (cursor_position_x >= best_video_mode.width-MARGIN_LEFT-LETTER_SIZE){
			move_cursor_to_next_line();
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

void print_backspace(){
	if (cursor_position_x>=LETTER_SIZE){
		cursor_position_x-=LETTER_SIZE;
	}
	else if (cursor_position_y>LETTER_SIZE && cursor_position_x==0){
		cursor_position_x = 0;
		cursor_position_y -= LETTER_SIZE;
	}
	unsigned int i;
	unsigned int j;
	for (i=0; i<LETTER_SIZE; i++){
		for (j=0; j<LETTER_SIZE; j++){
			put_pixel((u32)(cursor_position_x + j), (u32)(cursor_position_y + i), 0);
		}
	}
}

void print_tab (){
	int i;
	int number_of_spaces = 4;
	for (i=0; i<number_of_spaces; i++){
		print_string(" ");
	}
}

void print_cursor (){
	int i=0;
	int k=0;
	for (i=0; i<16; i++){
		int row = cursor[i];

		for (k =0; k<16; k++){
			u16 bit = (row & (1 << k));
			if (bit!=0){
				put_pixel ( 15-k, i, 0x00FFFFFF);
			}
		}


	}
}
