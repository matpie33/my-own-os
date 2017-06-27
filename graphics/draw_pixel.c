#include "draw_pixel.h"
#include "../cpu/types.h"
#include "../libc/hex_to_string.h"
#include "draw_string.h"
#include "../libc/printf.h"
#include "../libc/function.h"

uint32_t* back_buffer;
point dirty_area_starting_point;
point dirty_area_end_point;
boolean has_dirty_area;

void initialize (){
	dirty_area_starting_point= (point) {.x= best_video_mode.width, .y = best_video_mode.height};
	dirty_area_end_point = (point) {.x =0, .y = 0};
}

uint32_t calculate_offset(uint32_t x_pos, uint32_t y_pos){
	return y_pos*best_video_mode.bytes_per_line+(x_pos*(best_video_mode.bpp/8));
}

void repaint(){
	if (!has_dirty_area){
		return;
	}
	uint32_t row_to_repaint = calculate_offset(dirty_area_starting_point.x,
			dirty_area_starting_point.y);
	uint32_t numberOfRows = dirty_area_end_point.y-dirty_area_starting_point.y+1;
	uint32_t bytes_in_row = dirty_area_end_point.x-dirty_area_starting_point.x+1;
	uint32_t i;
	uint8_t bytes_per_pixel = best_video_mode.bpp/8;
	uint32_t bytes_to_copy = bytes_in_row*bytes_per_pixel;
	uint32_t address = (uint32_t)back_buffer;
	for (i=0; i<numberOfRows; i++){
		memory_copy((uint32_t*)(address+row_to_repaint),
				(uint32_t*)(best_video_mode.framebuffer+row_to_repaint), bytes_to_copy);
		row_to_repaint = row_to_repaint+best_video_mode.bytes_per_line;
	}
	has_dirty_area=false;
	initialize();

}

void initialize_back_buffer(){
	uint32_t numberOfBytes = best_video_mode.height*best_video_mode.width*best_video_mode.bpp/8;
	back_buffer = (uint32_t *) calloc(numberOfBytes, uint32);
	has_dirty_area=false;
}

void check_for_dirty_area(uint32_t x_pos, uint32_t y_pos){
	uint32_t end_point_offset = calculate_offset(dirty_area_end_point.x, dirty_area_end_point.y);
	uint32_t starting_point_offset = calculate_offset(dirty_area_starting_point.x, dirty_area_starting_point.y);
	uint32_t new_point_offset = calculate_offset(x_pos, y_pos);
	if (!has_dirty_area){
		has_dirty_area=true;
	}
	if (new_point_offset<starting_point_offset){
		dirty_area_starting_point = (point) {.x = x_pos, .y = y_pos};
	}
	if (new_point_offset > end_point_offset){
		dirty_area_end_point = (point) {.x = x_pos, .y = y_pos};
	}
}

uint32_t* put_pixel(uint32_t x_pos, uint32_t y_pos, uint32_t color){
	uint32_t current_offset = calculate_offset(x_pos, y_pos);
	uint32_t *video = (uint32_t*)((uint32_t)back_buffer+current_offset);
	*video=color;
	check_for_dirty_area(x_pos, y_pos);
	return video;
}

void draw_horizontal_line (uint32_t x_pos, uint32_t y_pos, uint32_t color, int length){
	uint32_t* video = put_pixel(x_pos, y_pos, color);
	int i=1;
	check_for_dirty_area(x_pos, y_pos);
	check_for_dirty_area(x_pos+length, y_pos);
	while (i<length){
		video[i]=color;

		i++;
	}
}

void draw_vertical_line (uint32_t x_pos, uint32_t y_pos, uint32_t color, uint32_t length){
	uint32_t* video = put_pixel(x_pos, y_pos, color);
	uint32_t i=0;
	check_for_dirty_area(x_pos, y_pos);
	check_for_dirty_area(x_pos, y_pos+length);
	while (i<=length*best_video_mode.bytes_per_line/4){
		video[i]=color;
		i+=best_video_mode.bytes_per_line/4;
	}
}

void fill_rectangle (uint32_t x_pos, uint32_t y_pos, uint32_t color, uint32_t width, uint32_t height){
	uint32_t* video = put_pixel(x_pos, y_pos, color);
	check_for_dirty_area(x_pos, y_pos);
	check_for_dirty_area(x_pos+width, y_pos+height);
	uint32_t i=0;
	for (i=0; i<height; i++){
		uint32_t j;
		for (j=0; j< width; j++){
			video[j] = color;
		}
		video+=best_video_mode.bytes_per_line/4;
	}
	repaint();
}

boolean object_can_be_drawn_at_position (uint16_t x_pos, uint16_t y_pos, uint16_t obj_width, uint16_t obj_height){
	return x_pos+obj_width <= best_video_mode.width && y_pos+obj_height <= best_video_mode.height;
}

void clear_area (uint16_t x_pos, uint16_t y_pos, uint16_t width, uint16_t height){
	int i=0;
	int j=0;
		for (i=0; i<width; i++){
			for (j =0; j<height; j++){
				put_pixel ( x_pos+i, y_pos+j, 0);
			}
		}
}

point get_center_of_screen_for_object(uint16_t object_width, uint16_t object_height){
	uint16_t x = best_video_mode.width/2-object_width/2;
	uint16_t y = best_video_mode.height/2 - object_height/2;
	point p;
	p.x=x;
	p.y=y;
	return p;
}
