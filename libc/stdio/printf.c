#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <kernel/tty.h>

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}


char* itoa(long long integer_to_convert, char buffer[]){
    char const digit[] = "0123456789";
    char* p = buffer;
    if(integer_to_convert<0){
        *p++ = '-';
        integer_to_convert *= -1;
    }
    long long shifter = integer_to_convert;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[integer_to_convert%10];
        integer_to_convert = integer_to_convert/10;
    }while(integer_to_convert);
    return buffer;
}

int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;
		if (format[0] == '\n'){
			format ++;
			written++;
			terminal_new_line();
			continue;
		}

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%' && format[amount] != '\n')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 'd') {
			format++;
			int c = (int) va_arg(parameters,  int);
			char char_buffer[32];
			char* chars = itoa((long long)c, char_buffer);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(chars, strlen(chars)))
				return -1;
			written++;
		} 
		else if (*format == 'l' && format[1] == 'l' && format[2] =='i') {			
			format +=3;
			long long c = ( long long) va_arg(parameters,  long long);
			char char_buffer[32];
			char* chars = itoa(c, char_buffer);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(chars, strlen(chars)))
				return -1;
			written++;			
			
		} 
		else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}

