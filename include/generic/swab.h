#ifndef	_GENERIC_SWAB_H_
#define	_GENERIC_SWAB_H_

// It seems that those do constant folding
// So ... why make it more complicated?

#define	__swab16(X)	__builtin_bswap16(X)
#define	__swab32(X)	__builtin_bswap32(X)
#define	__swab64(X)	__builtin_bswap64(X)

#endif
