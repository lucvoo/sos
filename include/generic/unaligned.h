#ifndef	_GENERIC_UNALIGNED_H_
#define	_GENERIC_UNALIGNED_H_


#ifdef	CONFIG_UNALIGNED_OK
#include <generic/unaligned-ok.h>
#elif	__BYTE_ORDER__ == 1234
#include <generic/unaligned-le.h>
#elif	__BYTE_ORDER__ == 4321
#include <generic/unaligned-be.h>
#endif

#endif
