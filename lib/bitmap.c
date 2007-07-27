#include <bitmap.h>


bool bitmap_is_empty(const unsigned long *src, int nbits)
{
	for (;nbits >= BITS_PER_LONG; nbits -= BITS_PER_LONG) {
		if (*src++ != 0)
			return false;
	}

	if (nbits)
		return (*src & BITMAP_MASK(nbits)) == 0;
	else
		return true;
}
