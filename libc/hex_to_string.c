#include "stdint.h"

#include "../cpu/types.h"
#include "mem.h"
#include "strings.h"

#define NUMBER_OF_CHARACTERS_FOR_32_BIT_HEX 11
#define HEX_PREFIX_0X_SIZE 2

void copyContents(char source[], char destination[], uint8_t size) {
	uint8_t i = 0;
	destination[0] = source[0];
	destination[1] = source[1];
	uint8_t array_length = strlen(source);
	uint8_t destination_length = strlen(destination);
	uint8_t startCopyingPosition = array_length - 1;
	uint8_t copyToStartingPosition = destination_length - 1;
	for (i = 0; i < size; i++) {
		destination[copyToStartingPosition - i] = source[startCopyingPosition - i];
	}
}

char * hex_to_string(uint32_t hexValue) {
	static char result[NUMBER_OF_CHARACTERS_FOR_32_BIT_HEX];
	uint8_t index = 0;
	result[index] = '0';
	index++;
	result[index] = 'x';
	index++;
	int i;
	uint32_t hexOriginal = hexValue;
	if (hexValue == 0x0) {
		//TODO maybe a better way
		uint8_t hexDigits = 1;
		char* newResult = (char*) malloc(HEX_PREFIX_0X_SIZE + hexDigits + 1, uint8);
		newResult[0] = '0';
		newResult[1] = 'x';
		newResult[2] = '0';
		return newResult;
	}
	for (i = NUMBER_OF_CHARACTERS_FOR_32_BIT_HEX - 2; i >= index; i--) {
		uint8_t digit = hexValue & 0x000f;
		digit += 0x30;
		if (digit > 0x39) {
			digit += 7;
		}
		char d = digit;
		result[i] = d;
		hexValue = hexValue >> 4;
	}
	result[NUMBER_OF_CHARACTERS_FOR_32_BIT_HEX - 1] = '\0';
	uint32_t rangeEnd = 0xFFFFFFFF;
	uint32_t rangeStart = 0x10000000;
	uint8_t hexDigitsAmount = 8;

	while (rangeStart > 0) {
		if (hexOriginal >= rangeStart && hexOriginal <= rangeEnd) {
			uint8_t arraySize = HEX_PREFIX_0X_SIZE + hexDigitsAmount + 1;
			char* newResult = (char*) malloc(arraySize, uint8);
			for (i = 0; i < arraySize - 1; i++) {
				newResult[i] = '1'; //anything can go here
			}
			newResult[arraySize - 1] = '\0';
			copyContents(result, newResult, hexDigitsAmount);
			return newResult;
		}
		else {
			rangeStart = rangeStart >> 4;
			rangeEnd = rangeStart << 4;
			hexDigitsAmount--;
		}
	}

	return result;

}

