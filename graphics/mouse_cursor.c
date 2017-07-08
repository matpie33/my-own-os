#include "stdint.h"

#include "../cpu/types.h"
#include "../libc/printf.h"
#include "../util/bit_handling.h"
#include "draw_pixel.h"

#define MOUSE_CURSOR_WIDTH 16
#define MOUSE_CURSOR_HEIGHT 16

const short cursor[MOUSE_CURSOR_HEIGHT] = { 0xc000, 0xb000, 0x4c00, 0x4380, 0x2060,
		0x2018, 0x1006, 0x1001, 0x0801, 0x0806, 0x0404, 0x0402, 0x0221, 0x02d1, 0x0109,
		0x0006 };

uint32_t* mouse_pixels_bytes[MOUSE_CURSOR_WIDTH];
uint32_t bytes_per_each_mouse_cursor_row;

uint16_t mouse_x_position;
uint16_t mouse_y_position;
uint16_t max_x_position;
uint16_t max_y_position;
uint16_t min_x_position;
uint16_t min_y_position;

void draw_cursor() {
	int i = 0;
	int k = 0;
	for (i = 0; i < 16; i++) {
		short row = cursor[i];
		for (k = 0; k < 16; k++) {
			uint16_t bit = is_bit_set(row, k);
			if (bit != 0) {
				put_pixel(mouse_x_position + MOUSE_CURSOR_WIDTH - 1 - k,
						mouse_y_position + i, 0x00FFFFFF);
			}
		}
	}
}

void draw_cursor_using_ready_bytes() {
	point p;
	p.x = mouse_x_position; //TODO copy only non zero bytes
	p.y = mouse_y_position;
	repaint_using_ready_bytes(mouse_pixels_bytes, MOUSE_CURSOR_WIDTH,
			bytes_per_each_mouse_cursor_row, p);
}

void clear_cursor() {
	clear_area(mouse_x_position, mouse_y_position, MOUSE_CURSOR_WIDTH,
	MOUSE_CURSOR_HEIGHT);
}

void initialize_cursor() {
	point screen_center = get_center_of_screen_for_object(MOUSE_CURSOR_WIDTH,
	MOUSE_CURSOR_HEIGHT);
	mouse_x_position = screen_center.x;
	mouse_y_position = screen_center.y;
	max_x_position = best_video_mode.width - MOUSE_CURSOR_WIDTH;
	max_y_position = best_video_mode.height - MOUSE_CURSOR_HEIGHT;
	min_x_position = 0;
	min_y_position = 0;
	draw_cursor();
	bytes_per_each_mouse_cursor_row = repaint_and_remember_pixels(mouse_pixels_bytes);
}

void move_mouse_on_screen(uint16_t new_x_pos, uint16_t new_y_pos) {
	clear_cursor();
	mouse_x_position = new_x_pos;
	mouse_y_position = new_y_pos;
	draw_cursor_using_ready_bytes();
}

boolean mouse_moved_outside_right_edge(int8_t pixels_to_move) {
	return pixels_to_move > 0 && mouse_x_position == max_x_position;
}

boolean mouse_moved_outside_left_edge(int8_t pixels_to_move) {
	return pixels_to_move < 0 && mouse_x_position == min_x_position;
}

boolean mouse_moved_outside_top_edge(int8_t pixels_to_move) {
	return pixels_to_move < 0 && mouse_y_position == min_y_position;
}

boolean mouse_moved_outside_bottom_edge(int8_t pixels_to_move) {
	return pixels_to_move > 0 && mouse_y_position == max_y_position;
}

void move_cursor_horizontally(int8_t pixels_to_move) {
	//	printf("pix: %d", pixels_to_move);
	if (mouse_moved_outside_right_edge(pixels_to_move)
			|| mouse_moved_outside_left_edge(pixels_to_move)) {
		return;
	}
	if (object_can_be_drawn_at_position(mouse_x_position + pixels_to_move,
			mouse_y_position,
			MOUSE_CURSOR_WIDTH, MOUSE_CURSOR_HEIGHT)) {
		move_mouse_on_screen(mouse_x_position + pixels_to_move, mouse_y_position);
	}
	else {
		int16_t destination_x_pos = mouse_x_position + pixels_to_move;
		if (destination_x_pos > max_x_position) {
			destination_x_pos = max_x_position;
		}
		else if (destination_x_pos < min_x_position) {
			destination_x_pos = min_x_position;
		}
		move_mouse_on_screen(destination_x_pos, mouse_y_position);
	}
}

void move_cursor_vertically(int8_t pixels_to_move) {
	if (mouse_moved_outside_top_edge(pixels_to_move)
			|| mouse_moved_outside_bottom_edge(pixels_to_move)) {
		return;
	}
	if (object_can_be_drawn_at_position(mouse_x_position,
			mouse_y_position + pixels_to_move,
			MOUSE_CURSOR_WIDTH, MOUSE_CURSOR_HEIGHT)) {
		move_mouse_on_screen(mouse_x_position, mouse_y_position + pixels_to_move);
	}
	else {
		int16_t destination_y_pos = mouse_y_position + pixels_to_move;
		if (destination_y_pos > max_y_position) {
			destination_y_pos = max_y_position;
		}
		else if (destination_y_pos < min_y_position) {
			destination_y_pos = min_y_position;
		}
		move_mouse_on_screen(mouse_x_position, destination_y_pos);
	}
}

