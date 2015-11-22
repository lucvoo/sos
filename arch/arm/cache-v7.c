#include <arch/cacheflush.h>
#include <arch/cp15.h>


static ulong cacheline_size(void)
{
	ulong ctr;

	asm ("mrc " STRINGIFY(CTR(%0)) : "=r" (ctr));

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
		asm ("mcr " STRINGIFY(OP(%0)) :: "r" (addr));	\
								\
		addr += line;					\
	} while (addr < end);					\
								\
	asm ("dsb	st");					\
}

DCACHE_OP_RANGE(dcache_clean_range, DCCMVAC)
DCACHE_OP_RANGE(dcache_inval_range, DCIMVAC)
DCACHE_OP_RANGE(dcache_flush_range, DCCIMVAC)
