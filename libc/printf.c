#include "stdarg.h"
#include "stdint.h"

#include "../cpu/types.h"
#include "../graphics/draw_string.h"
#include "hex_to_string.h"
#include "mem.h"
#include "strings.h"

#define BUFFER_SIZE 1024

int copy_int(char* indCharacters, char* destination, int destinationStartIndex) {
	int i;
	for (i = 0; i < strlen(indCharacters); i++) {
		destination[destinationStartIndex] = indCharacters[i];
		destinationStartIndex++;
	}
	return destinationStartIndex;
}

char* read_the_formatted_string (va_list list, char* format){
	//TODO we need a list of allocated buffers, so implement a list some day
		char* buffer = (char*) malloc(BUFFER_SIZE, uint8);
		int bufferIndex = 0;

		int i;
		for (i = 0; i < strlen(format); i++) {
			if (format[i] == '%') {
				char c = format[i + 1];
				i++;
				switch (c) {
					case 'd': {
						int digit = va_arg(list, int);
						char integer_characters[10];
						int_to_ascii(digit, integer_characters);
						bufferIndex = copy_int(integer_characters, buffer, bufferIndex);
						break;
					}
					case 'u': {
						uint8_t digit = va_arg(list, int);
						char integerCharacters[10];
						int_to_ascii((int)digit, integerCharacters);
						bufferIndex = copy_int(integerCharacters, buffer, bufferIndex);
						break;
					}
					case 's': {
						char* string = va_arg(list, char*);
						bufferIndex = copy_int(string, buffer, bufferIndex);
						break;
					}
					case 'c': {
						int letter = va_arg(list, int);
						char c[2];
						c[0] = letter;
						c[1] = '\0';
						buffer[bufferIndex] = letter;
						bufferIndex = copy_int(c, buffer, bufferIndex);
						break;
					}
					case 'x': {
						int test = va_arg(list, int);
						char* hex_string = hex_to_string(test);
						bufferIndex = copy_int(hex_string, buffer, bufferIndex);
						break;
					}
				}
			}
			else {
				buffer[bufferIndex] = format[i];
				bufferIndex++;
				if (bufferIndex >= BUFFER_SIZE) {
					bufferIndex = 0;
				}
			}
		}
		buffer[bufferIndex] = '\0';
		return buffer;
}

void printfln(char* format, ...) {
	va_list list;
	va_start(list, format);
	char* buffer = read_the_formatted_string(list, format);
	println(buffer);
}

void printf(char* format, ...) {
	va_list list;
	va_start(list, format);
	char* buffer = read_the_formatted_string(list, format);
	print_string(buffer);
}

