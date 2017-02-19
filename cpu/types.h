#ifndef TYPES_H
#define TYPES_H

typedef unsigned int u32;
typedef 		 int s32;
typedef unsigned short u16;
typedef 		 short s16;
typedef unsigned char u8;
typedef 		 char s8;

/* makro - podmienia to co przed spacją na to co po spacji - patrz gcc macros*/
#define low_16(address) (u16)((address) & 0xFFFF) // adres AND 0xFFFF powoduje że zatrzymujemy tylko 2 bajty od prawej, bo nasz adres może mieć więcej niż 2 bajty
#define high_16(address) (u16)(((address) >> 16) & 0xFFFF) // przesuwamy 16 bitów (2 bajty) ktore nas interesują na prawo, i robimy to co wyżej

#endif
