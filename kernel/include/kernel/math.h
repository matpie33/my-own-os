#ifndef MATH_H
#define MATH_H

#include <stdint.h>

uint32_t divide_round_up(uint32_t a, uint32_t b);
uint32_t log2(uint32_t number);
uint32_t power(uint32_t number, uint32_t exponent);
uint32_t min(uint32_t a, uint32_t b);

#endif