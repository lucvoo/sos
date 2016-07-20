#include <math.h>

unsigned long sqrtl(unsigned long n)
{
	unsigned long r = 0;
	unsigned long b = 1UL << (sizeof(n) * 8 - 2);

	// b = the highest power of 4 not greater than n
	while (b > n)
		b >>= 2;

	while (b != 0) {
		if (n >= r + b) {
			n -= r + b;
			r = (r >> 1) + b;
		} else
			r >>= 1;
		b >>= 2;
	}

	return r;
}

unsigned long sqrtll(unsigned long long n)
{
	unsigned long long r = 0;
	unsigned long long b = 1ULL << (sizeof(n) * 8 - 2);

	// b = the highest power of 4 not greater than n
	while (b > n)
		b >>= 2;

	while (b != 0) {
		if (n >= r + b) {
			n -= r + b;
			r = (r >> 1) + b;
		} else
			r >>= 1;
		b >>= 2;
	}

	return r;
}
