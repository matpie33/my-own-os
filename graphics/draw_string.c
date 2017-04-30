#include "../cpu/types.h"
#include "draw_pixel.h"
#include "font_encoding.h"

unsigned int cursor_position_x = 0;
unsigned int cursor_position_y = 0;

#define MARGIN_LEFT (unsigned int) 10
#define MARGIN_UP 10
#define PADDING_WITHIN_LINES 5

void move_cursor_to_next_line(){
	cursor_position_y +=LETTER_SIZE+PADDING_WITHIN_LINES;
	cursor_position_x = 0;
}

boolean cursor_within_n_characters_from_end_of_line(int n){
	return cursor_position_x >= best_video_mode.width-MARGIN_LEFT-n*LETTER_SIZE;
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
		if (cursor_within_n_characters_from_end_of_line(1)){
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
			//TODO copy pasted;
			put_pixel((u32)(cursor_position_x + j), (u32)(cursor_position_y + i), 0);
		}
	}
}

void print_tab (){
	int i;
	int number_of_spaces = 4;
	if (cursor_within_n_characters_from_end_of_line(number_of_spaces)){
		move_cursor_to_next_line();
	}
	for (i=0; i<number_of_spaces; i++){
		print_string(" ");
	}
}


