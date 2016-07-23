#include <arch/cacheflush.h>
#include <arch/barrier.h>
#include <arch/cp15.h>


static ulong cacheline_size(void)
{
	return 32;
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
	dsb(st);						\
}

DCACHE_OP_RANGE(dcache_clean_range, DCCMVAC)
DCACHE_OP_RANGE(dcache_inval_range, DCIMVAC)
DCACHE_OP_RANGE(dcache_flush_range, DCCIMVAC)
