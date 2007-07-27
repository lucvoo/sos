#ifndef	_BITMAP_H_
#define	_BITMAP_H_

#include <types.h>
#include <stdbool.h>


#define	BITMAP_DECLARE(name,bits) \
	unsigned long name[((bits)+BITS_PER_LONG-1)/BITS_PER_LONG]


#define BITMAP_MASK(nbits)		/* <nbits> must be between 0 & BITS_PER_LONG */ \
	(~0UL>>(BITS_PER_LONG - (nbits)))


bool bitmap_is_empty(const unsigned long *src, int nbits);

#endif
