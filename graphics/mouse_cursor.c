#include "draw_pixel.h"

#define MOUSE_CURSOR_WIDTH 16
#define MOUSE_CURSOR_HEIGHT 16

const short cursor [MOUSE_CURSOR_HEIGHT] = {
	0xc000,0xb000,0x4c00,0x4380,0x2060,0x2018,0x1006,0x1001,0x0801,0x0806,0x0404,0x0402,0x0221,
	0x02d1,0x0109,0x0006
};

u16 mouse_x_position;
u16 mouse_y_position;


void draw_cursor (){
	int i=0;
	int k=0;
	for (i=0; i<16; i++){
		int row = cursor[i];
		for (k =0; k<16; k++){
			u16 bit = (row & (1 << k));
			if (bit!=0){
				put_pixel ( mouse_x_position+MOUSE_CURSOR_WIDTH-1-k, mouse_y_position+i,
						0x00FFFFFF);
			}
		}
	}
}

void initialize_cursor (u16 x_pos, u16 y_pos){
	mouse_x_position = x_pos;
	mouse_y_position = y_pos;
	draw_cursor();
}

void clear_cursor (){
	clear_area(mouse_x_position, mouse_y_position,MOUSE_CURSOR_WIDTH, MOUSE_CURSOR_HEIGHT);
}

void move_cursor_right (u16 pixels_to_move){
	if (object_can_be_drawn_at_position(mouse_x_position+pixels_to_move, mouse_y_position,
			MOUSE_CURSOR_WIDTH,	MOUSE_CURSOR_HEIGHT)){
		clear_cursor ();
		mouse_x_position+=pixels_to_move;
		draw_cursor();
	}
}

void move_cursor_left (u16 pixels_to_move){
	if (object_can_be_drawn_at_position(mouse_x_position-pixels_to_move, mouse_y_position,
			MOUSE_CURSOR_WIDTH,	MOUSE_CURSOR_HEIGHT)){
		clear_cursor ();
		mouse_x_position-=pixels_to_move;
		draw_cursor();
	}
}

void move_cursor_up (u16 pixels_to_move){
	if (object_can_be_drawn_at_position(mouse_x_position, mouse_y_position-pixels_to_move,
			MOUSE_CURSOR_WIDTH,	MOUSE_CURSOR_HEIGHT)){
		clear_cursor ();
		mouse_y_position-=pixels_to_move;
		draw_cursor();
	}
}

void move_cursor_down (u16 pixels_to_move){
	if (object_can_be_drawn_at_position(mouse_x_position, mouse_y_position+pixels_to_move,
			MOUSE_CURSOR_WIDTH,	MOUSE_CURSOR_HEIGHT)){
		clear_cursor ();
		mouse_y_position+=pixels_to_move;
		draw_cursor();
	}
}





