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
	int i=0;
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
