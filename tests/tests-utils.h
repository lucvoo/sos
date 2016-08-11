/* tests/sprng.c */

#include <stdint.h>
#include <prng.h>

static inline uint sprng(void)
{
	return prng32();
}
