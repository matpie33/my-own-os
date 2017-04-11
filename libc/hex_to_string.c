#include "../cpu/types.h"
#include "../drivers/screen.h"
#include "../libc/function.h"
#include "../libc/strings.h"

char * hex_to_string(u32 hexValue){
	static char result [6];
	result[5]='0';
	result[4]='x';
	int i;
	for (i=0; i<4; i++){
		u8 digit = hexValue & 0x000f;
		digit+=0x30;
		if (digit>0x39){
			digit+=7;
		}
		char d = digit;
		result[i]=d;
		hexValue = hexValue >>4;
	}
	reverse(result);

	print(result);
	return result;
}
