#include "draw_pixel.h"
#include "../cpu/types.h"
#include "../libc/hex_to_string.h"



u32* put_pixel(u32 x_pos, u32 y_pos, u32 color){
	u32 *video = (u32*)(best_video_mode.framebuffer+
			y_pos*best_video_mode.bytes_per_line+(x_pos*(best_video_mode.bpp/8)));
	*video=color;
	return video;
}

void draw_horizontal_line (u32 x_pos, u32 y_pos, u32 color, int length){
	u32* video = put_pixel(x_pos, y_pos, color);
	int i=1;
	while (i<length){
		video[i]=color;
		i++;
	}
}

void draw_vertical_line (u32 x_pos, u32 y_pos, u32 color, int length){
	u32* video = put_pixel(x_pos, y_pos, color);
	int i=0;
	while (i<length*best_video_mode.bytes_per_line/4){
		video[i]=color;
		i+=best_video_mode.bytes_per_line/4;
	}
}

boolean object_can_be_drawn_at_position (u16 x_pos, u16 y_pos, u16 obj_width, u16 obj_height){
	return x_pos+obj_width <= best_video_mode.width && y_pos+obj_height <= best_video_mode.height;
}



void clear_area (u16 x_pos, u16 y_pos, u16 width, u16 height){
	int i=0;
	int j=0;
		for (i=0; i<width; i++){
			for (j =0; j<height; j++){
				put_pixel ( x_pos+i, y_pos+j, 0);
			}
		}
}

point get_center_of_screen_for_object(u16 object_width, u16 object_height){
	u16 x = best_video_mode.width/2-object_width/2;
	u16 y = best_video_mode.height/2 - object_height/2;
	point p;
	p.x=x;
	p.y=y;
	return p;
}
