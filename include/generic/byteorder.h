#ifndef	_GENERIC_BYTEORDER_H_
#define	_GENERIC_BYTEORDER_H_

#if	__BYTE_ORDER__ == 4321
#include <generic/byteorder-be.h>
#endif
#if	__BYTE_ORDER__ == 1234
#include <generic/byteorder-le.h>
#endif

#endif
