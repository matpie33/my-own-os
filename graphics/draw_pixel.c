#include "draw_pixel.h"
#include "../cpu/types.h"

void put_pixel(int x_pos, int y_pos, int color){
	u32 *video = (u32*) best_video_mode.framebuffer;
	u32 offset = y_pos*best_video_mode.bytes_per_line+x_pos*(best_video_mode.bpp/8);
	video[offset]=color;
}
