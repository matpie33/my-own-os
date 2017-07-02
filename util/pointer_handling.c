#include <stdint.h>

//TODO we assume 32 bits size here, how to do it more elastically
void replace_pointers_if_non_zero(uint32_t* to_replace, uint32_t* replacement, uint32_t pointers_size){
	uint32_t i=0;
	for (i =0; i<pointers_size; i++){
		if (replacement[i] !=0){ //TODO not a good idea, background could be other byte than 0 i.e. not black
			to_replace[i] = replacement[i];
		}
	}
}
