/* Copy bytes from one place to another . */

void reverse (char str []);

void memory_copy ( char * source , char * dest , int no_bytes ) {
int i;
	for (i =0; i< no_bytes ; i ++) {
		*( dest + i) = *( source + i);
	}
}

void int_to_ascii(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0; 
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';
	

}
