#include "draw_pixel.h"
#include <stdint.h>
#include "../libc/mem.h"

uint32_t* back_buffer;
point dirty_area_starting_point;
point dirty_area_end_point;
boolean has_dirty_area;

void initialize_back_buffer() {
	uint32_t numberOfBytes = best_video_mode.height * best_video_mode.width
			* best_video_mode.bpp / 8;
	back_buffer = (uint32_t *) calloc(numberOfBytes, uint32);
	has_dirty_area = false;
}

void initialize() {
	dirty_area_starting_point = (point ) { .x = best_video_mode.width, .y =
					best_video_mode.height };
	dirty_area_end_point = (point ) { .x = 0, .y = 0 };
}

void initialize_graphics() {
	initialize();
	initialize_back_buffer();
}

uint32_t calculate_offset(uint32_t x_pos, uint32_t y_pos) {
	return y_pos * best_video_mode.bytes_per_line
			+ (x_pos * (best_video_mode.bytes_per_pixel));
}

uint32_t copy_bytes_to_front_buffer_and_update_row_to_repaint(
		uint32_t* address_of_bytes_to_copy, uint32_t bytes_to_copy,
		uint32_t row_to_repaint) {
	memory_copy(address_of_bytes_to_copy,
			(uint32_t*) (best_video_mode.framebuffer + row_to_repaint), bytes_to_copy);
	return row_to_repaint + best_video_mode.bytes_per_line;
}

void copy_bytes_from_back_to_front_buffer(dirty_area_bytes bytes_to_copy_struct,
		boolean remember_bytes, uint32_t* container_for_bytes[]) {
	uint32_t row_to_repaint = calculate_offset(dirty_area_starting_point.x,
			dirty_area_starting_point.y);
	uint32_t address = (uint32_t) back_buffer;
	uint32_t i;
	for (i = 0; i < bytes_to_copy_struct.number_of_rows; i++) {
		if (remember_bytes) {
			memory_copy((uint32_t*) (address + row_to_repaint), container_for_bytes[i],
					bytes_to_copy_struct.bytes_in_row);
		}
		row_to_repaint = copy_bytes_to_front_buffer_and_update_row_to_repaint(
				(uint32_t*) (address + row_to_repaint), bytes_to_copy_struct.bytes_in_row,
				row_to_repaint);
	}
}

dirty_area_bytes calculate_number_of_bytes_to_copy() {
	uint32_t number_of_rows = dirty_area_end_point.y - dirty_area_starting_point.y + 1;
	uint32_t bytes_in_row = dirty_area_end_point.x - dirty_area_starting_point.x + 1;
	dirty_area_bytes bytes;
	bytes.bytes_in_row = bytes_in_row;
	bytes.number_of_rows = number_of_rows;
	return bytes;
}

void repaint() {
	if (!has_dirty_area) {
		return;
	}
	dirty_area_bytes bytes = calculate_number_of_bytes_to_copy();
	uint32_t* no_pointers[0]; //TODO how to workaround this? maybe create another method that takes
							  //no pointer and calls this method
	copy_bytes_from_back_to_front_buffer(bytes, false, no_pointers);
	has_dirty_area = false;
	initialize();
}

void repaint_using_ready_bytes(uint32_t* ready_bytes[], uint32_t array_size,
		uint32_t pointer_size, point point) {
	uint32_t row_to_repaint = calculate_offset(point.x, point.y);
	repaint();
	uint32_t i;
	for (i = 0; i < array_size; i++) {
		row_to_repaint = copy_bytes_to_front_buffer_and_update_row_to_repaint(
				ready_bytes[i], pointer_size, row_to_repaint);
	}
}

uint32_t repaint_and_remember_pixels(uint32_t* container_for_bytes[]) {
	if (!has_dirty_area) {
		return 0;
	}
	dirty_area_bytes bytes = calculate_number_of_bytes_to_copy();
	uint32_t i;
	for (i = 0; i < bytes.number_of_rows; i++) {
		container_for_bytes[i] = (uint32_t*) malloc(bytes.bytes_in_row, uint32);
	}
	copy_bytes_from_back_to_front_buffer(bytes, true, container_for_bytes);
	has_dirty_area = false;
	initialize();
	return bytes.bytes_in_row;
}

void check_for_dirty_area(uint32_t x_pos, uint32_t y_pos) {
	if (!has_dirty_area) {
		has_dirty_area = true;
	}
	if (x_pos < dirty_area_starting_point.x) {
		dirty_area_starting_point.x = x_pos;
	}
	if (y_pos < dirty_area_starting_point.y) {
		dirty_area_starting_point.y = y_pos;
	}
	if (x_pos > dirty_area_end_point.x) {
		dirty_area_end_point.x = x_pos;
	}
	if (y_pos > dirty_area_end_point.y) {
		dirty_area_end_point.y = y_pos;
	}
}

uint32_t* put_pixel(uint32_t x_pos, uint32_t y_pos, uint32_t color) {
	uint32_t current_offset = calculate_offset(x_pos, y_pos);
	uint32_t *video = (uint32_t*) ((uint32_t) back_buffer + current_offset);
	*video = color;
	check_for_dirty_area(x_pos, y_pos);
	return video;
}

void draw_horizontal_line(uint32_t x_pos, uint32_t y_pos, uint32_t color, int length) {
	uint32_t* video = put_pixel(x_pos, y_pos, color);
	int i = 1;
	check_for_dirty_area(x_pos, y_pos);
	check_for_dirty_area(x_pos + length, y_pos);
	while (i < length) {
		video[i] = color;

		i++;
	}
}

void draw_vertical_line(uint32_t x_pos, uint32_t y_pos, uint32_t color, uint32_t length) {
	uint32_t* video = put_pixel(x_pos, y_pos, color);
	uint32_t i = 0;
	check_for_dirty_area(x_pos, y_pos);
	check_for_dirty_area(x_pos, y_pos + length);
	while (i <= length * best_video_mode.bytes_per_line / 4) {
		video[i] = color;
		i += best_video_mode.bytes_per_line / 4;
	}
}

void fill_rectangle(uint32_t x_pos, uint32_t y_pos, uint32_t color, uint32_t width,
		uint32_t height) {
	uint32_t* video = put_pixel(x_pos, y_pos, color);
	check_for_dirty_area(x_pos, y_pos);
	check_for_dirty_area(x_pos + width, y_pos + height);
	uint32_t i = 0;
	uint16_t bytes_per_row = best_video_mode.bytes_per_line / 4;
	for (i = 0; i < height; i++) {
		uint32_t j;
		for (j = 0; j < width; j++) {
			video[j] = color;
		}
		video += bytes_per_row;
	}
	repaint();
}

boolean object_can_be_drawn_at_position(uint16_t x_pos, uint16_t y_pos,
		uint16_t obj_width, uint16_t obj_height) {
	return x_pos + obj_width <= best_video_mode.width
			&& y_pos + obj_height <= best_video_mode.height;
}

void clear_area(uint16_t x_pos, uint16_t y_pos, uint16_t width, uint16_t height) {
	fill_rectangle(x_pos, y_pos, 0, width, height);
}

point get_center_of_screen_for_object(uint16_t object_width, uint16_t object_height) {
	uint16_t x = best_video_mode.width / 2 - object_width / 2;
	uint16_t y = best_video_mode.height / 2 - object_height / 2;
	point p;
	p.x = x;
	p.y = y;
	return p;
}
