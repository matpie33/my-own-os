#ifndef DRAW_STRING_H
#define DRAW_STRING_H

void draw_string(char* a, uint32_t color);
void print_string(char* string);
void println (char* a);
void print_backspace();
void print_tab ();
void print_int (uint32_t value);
void print_hex (uint32_t value);
void print_pointer(uint32_t* pointer, uint32_t size);

#endif
