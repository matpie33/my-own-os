#include <stdint.h>

uint32_t power(uint32_t number, uint32_t exponent)
{
	uint32_t result = 1;
	for (uint32_t i = 0; i < exponent; i++)
	{
		result *= number;
	}
	return result;
}

uint32_t divide_round_up(uint32_t a, uint32_t b)
{
	return (a + b - 1) / b;
}

uint32_t log2(uint32_t number)
{
	uint32_t bit = 0;
	while (number >>= 1)
	{
		bit++;
	}
	return bit;
}

uint32_t min (uint32_t a, uint32_t b){
	return a < b ? a : b;
}
