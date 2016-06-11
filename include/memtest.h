#ifndef	_MEMTEST_H_
#define	_MEMTEST_H_

#define	MEMTEST_DUMP		0x1
#define	MEMTEST_VERBOSE		0x2

/* lib/memtest.c */
void memtest_probe(ulong addr, ulong size, u32 gran, int flags);
int  memtest_simple(ulong addr, ulong size, u32 gran, int flags);

#endif
