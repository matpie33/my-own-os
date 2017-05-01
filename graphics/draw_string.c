#include "../cpu/types.h"
#include "draw_pixel.h"
#include "font_encoding.h"

u16 text_cursor_position_x = 0;
u16 text_cursor_position_y = 0;

#define MARGIN_LEFT (unsigned int) 10
#define MARGIN_UP 10
#define PADDING_WITHIN_LINES 5

void move_cursor_to_next_line(){
	text_cursor_position_y +=LETTER_SIZE+PADDING_WITHIN_LINES;
	text_cursor_position_x = 0;
}

boolean cursor_within_n_characters_from_end_of_line(u8 n){
	return text_cursor_position_x >= best_video_mode.width-MARGIN_LEFT-n*LETTER_SIZE;
}

void draw_letter(u16 x_pos, u16 y_pos, u32 color){
	put_pixel((u32)(MARGIN_LEFT+x_pos), (u32)(MARGIN_UP+y_pos), color);
}

void draw_string(char* a, u32 color){
	u8 i=0;
	u8 j=0;
	u16 k=0;

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
					draw_letter(text_cursor_position_x + j, text_cursor_position_y + i, color);
				}

			}
		}
		text_cursor_position_x+=LETTER_SIZE;
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
	if (text_cursor_position_x>=LETTER_SIZE){
		text_cursor_position_x-=LETTER_SIZE;
	}
	else if (text_cursor_position_y>LETTER_SIZE && text_cursor_position_x==0){
		text_cursor_position_x = 0;
		text_cursor_position_y -= LETTER_SIZE;
	}
	unsigned int i;
	unsigned int j;
	for (i=0; i<LETTER_SIZE; i++){
		for (j=0; j<LETTER_SIZE; j++){
			draw_letter(text_cursor_position_x + j, text_cursor_position_y+i, 0);
		}
	}
}

void print_tab (){
	u8 i;
	u8 number_of_spaces = 4;
	if (cursor_within_n_characters_from_end_of_line(number_of_spaces)){
		move_cursor_to_next_line();
	}
	for (i=0; i<number_of_spaces; i++){
		print_string(" ");
	}
}


