#ifndef DRAW_PIXEL_H
#define DRAW_PIXEL_H

#include "../cpu/types.h"

typedef struct {
	u16 x;
	u16 y;
} point;

typedef struct {
	u8 bpp;
	u16 height;
	u16 width;
	u16 mode;
	u32 framebuffer;
	u16 bytes_per_line;
	u32 bytes_per_pixel;
	u16 x_cur_max;
	u16 y_cur_max;
} __attribute__((packed)) video_mode;

extern video_mode best_video_mode;

u32* put_pixel(u32 x_pos, u32 y_pos, u32 color);
void draw_horizontal_line (u32 x_pos, u32 y_pos, u32 color, int length);
void draw_vertical_line (u32 x_pos, u32 y_pos, u32 color, int length);
void clear_area (u16 x_pos, u16 y_pos, u16 width, u16 height);
boolean object_can_be_drawn_at_position (u16 x_pos, u16 y_pos, u16 obj_width, u16 obj_height);
point get_center_of_screen_for_object(u16 object_width, u16 object_height);

#endif
