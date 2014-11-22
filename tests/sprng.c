#include "tests-utils.h"

// Simple LCG: x = (Ax + C) mod M (with M=2^32)
// Hull-Dobel requirements:
// - C and M must be coprime
// - A - 1 must be divisible by all prime factors of M
// - A - 1 must be divisible by 4 if M is divisible by 4

#define A	0x4dbca6bd
#define C	0x8811434b
#define S	0x54509438

uint32_t sprng(void)
{
	static uint32_t x = S;

	x = (x * A) + C;

	return x;
}
