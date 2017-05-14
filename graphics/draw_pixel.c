#include "draw_pixel.h"
#include "../cpu/types.h"
#include "../libc/hex_to_string.h"
#include "draw_string.h"



uint32_t* put_pixel(uint32_t x_pos, uint32_t y_pos, uint32_t color){
	uint32_t *video = (uint32_t*)(best_video_mode.framebuffer+
			y_pos*best_video_mode.bytes_per_line+(x_pos*(best_video_mode.bpp/8)));
	*video=color;
	return video;
}

void draw_horizontal_line (uint32_t x_pos, uint32_t y_pos, uint32_t color, int length){
	uint32_t* video = put_pixel(x_pos, y_pos, color);
	int i=1;
	while (i<length){
		video[i]=color;
		i++;
	}
}

void draw_vertical_line (uint32_t x_pos, uint32_t y_pos, uint32_t color, int length){
	uint32_t* video = put_pixel(x_pos, y_pos, color);
	int i=0;
	while (i<length*best_video_mode.bytes_per_line/4){
		video[i]=color;
		i+=best_video_mode.bytes_per_line/4;
	}
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
