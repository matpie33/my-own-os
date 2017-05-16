#include <stdint.h>
//TODO #ifdef DEBUG
#include "hex_to_string.h"
#include "../graphics/draw_string.h"
#include "strings.h"
#include "../cpu/types.h"

void memory_copy ( uint32_t *source , uint32_t *dest , int no_bytes ) {
int i;
	for (i =0; i< no_bytes ; i ++) {
		*( dest + i) = *( source + i);
	}
}

void memory_set(uint8_t *dest, uint8_t val, uint32_t len) {
	uint8_t *temp = (uint8_t *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

uint32_t free_memory_start = 0x10000; // TODO learn how to use linker scripts to somehow read the kernel's
									  // location from bootloader
uint32_t malloc (uint32_t size_in_bytes){
	uint32_t returnedAddress = free_memory_start;
	free_memory_start+=size_in_bytes*8;
	return returnedAddress;
}

//TODO #ifdef DEBUG
void test_allocated_memory(uint32_t address, uint32_t size){
	uint32_t i=0;
	//TODO create printf method; print_hex method
	println("");
	print_string("testing memory: address: ");
	print_hex((address));
	print_string(" size: ");
	print_hex((size));
	println("");
	uint32_t* block = (uint32_t*) address;
	boolean noErrors = true;
	for (i=0; i<size; i++){
		uint32_t valueTested = *(block+i);
		if (valueTested != 0){
			print_string("Non zero element found. Position within block:");
			print_int(i);
			print_string("Value in hex: ");
			print_hex((valueTested));
			noErrors = false;
			break;
		}
	}
	if (noErrors){
		print_string("The block of memory has all zeroes");
	}
}
