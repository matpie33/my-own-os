#ifndef BACK_BUFFER_H
#define BACK_BUFFER_H

void initialize_back_buffer();
void write_to_back_buffer(uint32_t* values, int startPosition, int length);

#endif
