#include "../cpu/types.h"
#include "draw_pixel.h"
#include "font_encoding.h"
#include "../libc/hex_to_string.h"
#include "../libc/strings.h"

uint16_t text_cursor_position_x = 0;
uint16_t text_cursor_position_y = 0;

#define MARGIN_LEFT (unsigned int) 10
#define MARGIN_UP 10
#define PADDING_WITHIN_LINES 5

void move_cursor_to_next_line(){
	text_cursor_position_y +=LETTER_SIZE+PADDING_WITHIN_LINES;
	text_cursor_position_x = 0;
}

boolean cursor_within_n_characters_from_end_of_line(uint8_t n){
	return text_cursor_position_x >= best_video_mode.width-MARGIN_LEFT-n*LETTER_SIZE;
}

void draw_letter(uint16_t x_pos, uint16_t y_pos, uint32_t color){
	put_pixel((uint32_t)(MARGIN_LEFT+x_pos), (uint32_t)(MARGIN_UP+y_pos), color);
}

void draw_string(char* a, uint32_t color){
	uint8_t i=0;
	uint8_t j=0;
	uint16_t k=0;

	while (a[k]!=0){
		if (a[k]=='\n'){
			move_cursor_to_next_line();
			break;
		}
		for (i=0; i<LETTER_SIZE; i++){
			int row = font8x8_basic[(int)a[k]][i];
			for (j=0; j<LETTER_SIZE; j++){
				uint16_t bit = (row & (1 << j));
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
	uint8_t i;
	uint8_t number_of_spaces = 4;
	if (cursor_within_n_characters_from_end_of_line(number_of_spaces)){
		move_cursor_to_next_line();
	}
	for (i=0; i<number_of_spaces; i++){
		print_string(" ");
	}
}

void print_int (uint32_t value){
	char asc [10];
	int_to_ascii(value, asc);
	print_string(asc);
}

void print_hex (uint32_t value){
	print_string(hex_to_string(value));
	print_string(", ");
}

void print_pointer(uint32_t* pointer, uint32_t size){
	uint32_t i;
	for (i=0; i<size; i++){
			print_hex(*(pointer+i));
			print_string(",");
		}
}


