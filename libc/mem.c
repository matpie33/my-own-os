#include <stdint.h>
//TODO #ifdef DEBUG
#include "hex_to_string.h"
#include "../graphics/draw_string.h"
#include "strings.h"
#include "../cpu/types.h"
#include "../libc/printf.h"

void memory_copy ( uint32_t *source , uint32_t *dest , int no_bytes ) {
int i;
	for (i =0; i< no_bytes ; i ++) {
		*( dest + i) = *( source + i);
	}
}

void memory_set(uint32_t *dest, uint32_t val, uint32_t len) {
	uint32_t *temp = (uint32_t *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

uint32_t free_memory_start = 0x10000; // TODO learn how to use linker scripts to somehow read the kernel's
									  // location from bootloader
uint8_t get_variable_size (variable_type variable_type){
	uint8_t variable_size;
	switch (variable_type){
		case uint8:{
			variable_size = 1;
			break;
		}
		case uint16:{
			variable_size =2;
			break;
		}
		case uint32:{
			variable_size = 4;
			break;
		}
	}
	return variable_size;
}

uint32_t malloc (uint32_t required_space, variable_type variable_type){
	uint8_t variable_size = get_variable_size(variable_type);
	uint32_t returned_address = free_memory_start;
	free_memory_start+=variable_size*required_space;
	return returned_address;
}

uint32_t calloc (uint32_t required_space, variable_type variable_type){
	uint32_t returned_address = malloc(required_space, variable_type);
	uint8_t type_size = get_variable_size(variable_type);
	memory_set((uint32_t*)returned_address, 0, required_space/type_size);
	return returned_address;
}


//TODO #ifdef DEBUG
void test_allocated_memory(uint32_t address, uint32_t size){
	uint32_t i=0;
	println("");
	printf("testing memory: address: %x ", address);
	printf("size: %d ", size);
	println("");
	uint32_t* block = (uint32_t*) address;
	boolean noErrors = true;
	for (i=0; i<size; i++){
		uint32_t valueTested = *(block+i);
		if (valueTested != 0){
			printf("Non zero element found. Position within block: %d", i);
			printf("Value in hex: %x", valueTested);
			noErrors = false;
			break;
		}
	}
	if (noErrors){
		print_string("The block of memory has all zeroes");
	}
}
