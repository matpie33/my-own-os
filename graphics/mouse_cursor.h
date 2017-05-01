#ifndef MOUSE_CURSOR_H
#define MOUSE_CURSOR_H

void move_cursor_right (u16 pixels_to_move);
void move_cursor_left (u16 pixels_to_move);
void move_cursor_up (u16 pixels_to_move);
void move_cursor_down (u16 pixels_to_move);
void initialize_cursor(u16 x_pos, u16 y_pos);

#endif
