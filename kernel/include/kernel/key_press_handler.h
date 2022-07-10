#ifndef KEY_PRESS_HANDLER
#define KEY_PRESS_HANDLER

#include <stdbool.h>
#include <kernel/special_keys.h>
#include <stdint.h>

#define KEY_PRESSED 1
#define KEY_NOT_PRESSED 0

void handle_key(uint8_t scancode);
void set_display_pressed_keys_on_screen(bool display);

#endif