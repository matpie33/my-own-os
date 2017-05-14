#include <stdint.h>
#include "../libc/function.h"
#include "../libc/strings.h"
#define NUMBER_OF_CHARACTERS_FOR_32_BIT_HEX 11

char * hex_to_string(uint32_t hexValue){
	static char result [NUMBER_OF_CHARACTERS_FOR_32_BIT_HEX]; //TODO refactor - use variables instead of hardcoding numbers
	uint8_t index = 0;
	result[index]='0';
	index++;
	result[index]='x';
	index++;
	int i;
	for (i=NUMBER_OF_CHARACTERS_FOR_32_BIT_HEX-2; i>=index; i--){
		uint8_t digit = hexValue & 0x000f;
		digit+=0x30;
		if (digit>0x39){
			digit+=7;
		}
		char d = digit;
		result[i]=d;
		hexValue = hexValue >>4;
	}
	result[NUMBER_OF_CHARACTERS_FOR_32_BIT_HEX-1]='\0';

	return result;
}
