#include <arch/cacheflush.h>
#include <arch/cp15.h>


static ulong cacheline_size(void)
{
	ulong ctr = cp_read(CTR);

	return ((ctr >> 16) & 0xf) * 4;
}

#define	DCACHE_OP_RANGE(NAME, OP)				\
void NAME(void *vaddr, uint size)				\
{								\
	ulong line = cacheline_size();				\
	ulong addr = (ulong) vaddr;				\
	ulong end = addr + size;				\
								\
	addr = addr & ~(line - 1);				\
	do {							\
		cp_write(OP, addr);				\
								\
		addr += line;					\
	} while (addr < end);					\
								\
	asm ("dsb	st");					\
}

DCACHE_OP_RANGE(dcache_clean_range, DCCMVAC)
DCACHE_OP_RANGE(dcache_inval_range, DCIMVAC)
DCACHE_OP_RANGE(dcache_flush_range, DCCIMVAC)
