#ifndef DRAW_PIXEL_H
#define DRAW_PIXEL_H

#include <stdint.h>
#include "../cpu/types.h"

typedef struct {
	uint16_t x;
	uint16_t y;
} point;

typedef struct {
	uint8_t bpp;
	uint16_t height;
	uint16_t width;
	uint16_t mode;
	uint32_t framebuffer;
	uint16_t bytes_per_line;
	uint32_t bytes_per_pixel;
	uint16_t x_cur_max;
	uint16_t y_cur_max;
} __attribute__((packed)) video_mode;

extern video_mode best_video_mode;

uint32_t* put_pixel(uint32_t x_pos, uint32_t y_pos, uint32_t color);
void draw_horizontal_line (uint32_t x_pos, uint32_t y_pos, uint32_t color, int length);
void draw_vertical_line (uint32_t x_pos, uint32_t y_pos, uint32_t color, int length);
void clear_area (uint16_t x_pos, uint16_t y_pos, uint16_t width, uint16_t height);
boolean object_can_be_drawn_at_position (uint16_t x_pos, uint16_t y_pos, uint16_t obj_width, uint16_t obj_height);
point get_center_of_screen_for_object(uint16_t object_width, uint16_t object_height);

#endif
