#ifndef STRINGS_H
#define STRINGS_H

#include "stdint.h"

void int_to_ascii(int n, char str[]);
void reverse(char s[]);
int strlen(char s[]);
void append(char s[], char n);
void backspace(char s[]);
int strcmp(char s1[], char s2[]);

#endif
