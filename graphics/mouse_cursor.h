#ifndef MOUSE_CURSOR_H
#define MOUSE_CURSOR_H

void move_cursor_right (uint16_t pixels_to_move);
void move_cursor_left (uint16_t pixels_to_move);
void move_cursor_up (uint16_t pixels_to_move);
void move_cursor_down (uint16_t pixels_to_move);
void initialize_cursor(uint16_t x_pos, uint16_t y_pos);

#endif
