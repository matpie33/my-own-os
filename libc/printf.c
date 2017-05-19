#include "../graphics/draw_string.h"
#include "strings.h"
#include <stdarg.h>
#include "../libc/mem.h"
#define BUFFER_SIZE 1024

int copy_int(char* indCharacters, char* destination, int destinationStartIndex){
int i;
	for (i=0; i<strlen(indCharacters); i++){
		destination[destinationStartIndex] = indCharacters[i];
		destinationStartIndex++;
	}
	return destinationStartIndex;
}

void printf(char* format, ...){

	va_list list;
	va_start (list, format);

	//TODO we need a list of allocated buffers, so implement a list some day
	char buffer[BUFFER_SIZE];
	int bufferIndex=0;

	int i;
	for (i=0; i<strlen(format); i++){
		if (format[i] == '%'){
			char c = format[i+1];
			i++;
			switch (c){
				case 'd':{
					int digit=va_arg(list, int);
					char integerCharacters [10];
					int_to_ascii(digit, integerCharacters);
					bufferIndex = copy_int(integerCharacters, buffer, bufferIndex);
					break;
				}
				case 's':{
					char* string = va_arg(list, char*);
					bufferIndex = copy_int(string, buffer, bufferIndex);
					break;
				}
				case 'c':{
					int test = va_arg(list, int);
					char c[2];
					c[0]=test;
					c[1]='\0';
					buffer[bufferIndex]=test;
					bufferIndex = copy_int (c, buffer, bufferIndex);
				}
			}
		}
		else{
			buffer[bufferIndex]=format[i];
			bufferIndex++;
			if (bufferIndex>=BUFFER_SIZE){
				bufferIndex=0;
			}
		}
	}
	buffer[bufferIndex]='\0';
	println(buffer);
}


