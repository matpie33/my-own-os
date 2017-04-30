#include "draw_pixel.h"
#include "../cpu/types.h"
#include "../libc/hex_to_string.h"

const short cursor [16] = {
		0xf000,	0x9e00,	0x83c0,	0x80c0,	0x8100,	0x8200,	0x9100,	0xa880,	0xc440,	0x8220,	0x0110,	0x0088,
		0x0044,	0x0022,	0x0014,	0x0008
};

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

boolean object_can_be_drawn_at_position (int x_pos, int y_pos, int obj_width, int obj_height){
	return x_pos+obj_width <= best_video_mode.width && y_pos+obj_height <= best_video_mode.height;
}

void print_cursor (int x_pos, int y_pos){
	int i=0;
	int k=0;
	if (!object_can_be_drawn_at_position(x_pos, y_pos, 16, 16)){
		return;
	}
	for (i=0; i<16; i++){
		int row = cursor[i];
		for (k =0; k<16; k++){
			u16 bit = (row & (1 << k));
			if (bit!=0){
				put_pixel ( x_pos+15-k, y_pos+i, 0x00FFFFFF);
			}
		}
	}
}

point get_center_of_screen_for_object(int object_width, int object_height){
	//TODO dont waste memory; maybe shorts are better instead of ints? check also other files.
	int x = best_video_mode.width/2-object_width/2;
	int y = best_video_mode.height/2 - object_height/2;
	point p;
	p.x=x;
	p.y=y;
	return p;
}
