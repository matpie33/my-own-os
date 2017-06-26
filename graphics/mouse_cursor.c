#include "draw_pixel.h"
#include "../libc/printf.h"
#include "../graphics/draw_string.h"

#define MOUSE_CURSOR_WIDTH 16
#define MOUSE_CURSOR_HEIGHT 16

const short cursor [MOUSE_CURSOR_HEIGHT] = {
	0xc000,0xb000,0x4c00,0x4380,0x2060,0x2018,0x1006,0x1001,0x0801,0x0806,0x0404,0x0402,0x0221,
	0x02d1,0x0109,0x0006
};

uint16_t mouse_x_position;
uint16_t mouse_y_position;
uint16_t max_x_position;
uint16_t max_y_position;
uint16_t min_x_position;
uint16_t min_y_position;


void draw_cursor (){
	int i=0;
	int k=0;
	for (i=0; i<16; i++){
		int row = cursor[i];
		for (k =0; k<16; k++){
			uint16_t bit = (row & (1 << k)); //TODO use my utils function instead
			if (bit!=0){
				put_pixel ( mouse_x_position+MOUSE_CURSOR_WIDTH-1-k, mouse_y_position+i,
						0x00FFFFFF);
			}
		}
	}
}

void clear_cursor (){
	clear_area(mouse_x_position, mouse_y_position,MOUSE_CURSOR_WIDTH, MOUSE_CURSOR_HEIGHT);
}

void initialize_cursor (uint16_t x_pos, uint16_t y_pos){
	mouse_x_position = x_pos;
	mouse_y_position = y_pos;
	max_x_position = best_video_mode.width - MOUSE_CURSOR_WIDTH;
	max_y_position = best_video_mode.height - MOUSE_CURSOR_HEIGHT;
	min_x_position = 0;
	min_y_position = 0;
	draw_cursor();

	repaint();
}



void move_mouse_on_screen(uint8_t new_x_pos, uint8_t new_y_pos){
	clear_cursor ();
	mouse_x_position = new_x_pos;
	mouse_y_position = new_y_pos;
	draw_cursor();
	repaint();
}

boolean mouse_moved_outside_right_edge (int8_t pixels_to_move){
	return pixels_to_move>0 && mouse_x_position == max_x_position;
}

boolean mouse_moved_outside_left_edge (int8_t pixels_to_move){
	return pixels_to_move<0 && mouse_x_position == min_x_position;
}

boolean mouse_moved_outside_top_edge (int8_t pixels_to_move){
	return pixels_to_move<0 && mouse_y_position == min_y_position;
}

boolean mouse_moved_outside_bottom_edge (int8_t pixels_to_move){
	return pixels_to_move>0 && mouse_y_position == max_y_position;
}

void move_cursor_horizontally (int8_t pixels_to_move){
//	printf("pix: %d", pixels_to_move);
	if (mouse_moved_outside_right_edge(pixels_to_move) || mouse_moved_outside_left_edge(pixels_to_move)){
		return;
	}
	if (object_can_be_drawn_at_position(mouse_x_position + pixels_to_move, mouse_y_position,
			MOUSE_CURSOR_WIDTH,	MOUSE_CURSOR_HEIGHT)){

//		move_mouse_on_screen(mouse_x_position+pixels_to_move, mouse_y_position);
		clear_cursor ();
		mouse_x_position+=pixels_to_move;

		draw_cursor();
		repaint();
	}
	else{
		clear_cursor ();
		uint8_t destination_x_pos = mouse_x_position + pixels_to_move;
		if (destination_x_pos > max_x_position){
			mouse_x_position = max_x_position;
		}
		else if (destination_x_pos < min_x_position){
			mouse_x_position = min_x_position;
		}

		draw_cursor();
		repaint();
	}
}


void move_cursor_vertically (int8_t pixels_to_move){
	if (mouse_moved_outside_top_edge(pixels_to_move) || mouse_moved_outside_bottom_edge(pixels_to_move)){
		return;
	}
	if (object_can_be_drawn_at_position(mouse_x_position, mouse_y_position+pixels_to_move,
			MOUSE_CURSOR_WIDTH,	MOUSE_CURSOR_HEIGHT)){

		clear_cursor ();
		//TODO 2 when there's a lot of text printed, repainting cause clearing more space
		// horizontally than it should

		mouse_y_position+=pixels_to_move;
		draw_cursor();
		repaint();
	}
	else{
		uint8_t destination_y_pos = mouse_y_position + pixels_to_move;
		clear_cursor ();
		if (destination_y_pos > max_y_position){
			mouse_y_position = max_y_position;
		}
		else if (destination_y_pos < min_y_position){
			mouse_y_position = min_y_position;
		}
		draw_cursor();
		repaint();
	}
}






