#include <arch/cacheop.h>
#include <arch/memory.h>
#include <arch/cacheflush.h>
#include <soc/cache.h>
#include <page.h>


#define	UNROLL	16

static inline void cache_op(ulong addr, uint op)
{
	asm("\n"
	"	cache %1, 0x000(%0)\n"
	: : "r" (addr), "i" (op));
}


/**
 * unrolled cache operations for cacheline size == 32
 */
static inline_always void cache_unroll32(ulong addr, uint op)
{
	asm("\n"
	"	.set push\n"
	"	.set noreorder\n"
	"	cache %1, 0x000(%0)\n"
	"	cache %1, 0x020(%0)\n"
	"	cache %1, 0x040(%0)\n"
	"	cache %1, 0x060(%0)\n"
	"	cache %1, 0x080(%0)\n"
	"	cache %1, 0x0a0(%0)\n"
	"	cache %1, 0x0c0(%0)\n"
	"	cache %1, 0x0e0(%0)\n"
	"	cache %1, 0x100(%0)\n"
	"	cache %1, 0x120(%0)\n"
	"	cache %1, 0x140(%0)\n"
	"	cache %1, 0x160(%0)\n"
	"	cache %1, 0x180(%0)\n"
	"	cache %1, 0x1a0(%0)\n"
	"	cache %1, 0x1c0(%0)\n"
	"	cache %1, 0x1e0(%0)\n"
#if (UNROLL >= 32)
	"	cache %1, 0x200(%0)\n"
	"	cache %1, 0x220(%0)\n"
	"	cache %1, 0x240(%0)\n"
	"	cache %1, 0x260(%0)\n"
	"	cache %1, 0x280(%0)\n"
	"	cache %1, 0x2a0(%0)\n"
	"	cache %1, 0x2c0(%0)\n"
	"	cache %1, 0x2e0(%0)\n"
	"	cache %1, 0x300(%0)\n"
	"	cache %1, 0x320(%0)\n"
	"	cache %1, 0x340(%0)\n"
	"	cache %1, 0x360(%0)\n"
	"	cache %1, 0x380(%0)\n"
	"	cache %1, 0x3a0(%0)\n"
	"	cache %1, 0x3c0(%0)\n"
	"	cache %1, 0x3e0(%0)\n"
#endif
	"	.set pop"
	: : "r" (addr), "i" (op));
}


static inline_always void cache_unroll(ulong addr, uint op, uint lsize)
{
	extern void cache_unroll_link_error(void);

	switch (lsize) {
	case 32:
		cache_unroll32(addr, op);
		break;
	default:
		cache_unroll_link_error();
	}
}


static inline_always void __cache_range_op(uint op, uint lsize, ulong start, ulong size)
{
	ulong mask = ~(lsize - 1);
	ulong addr = start & mask;
	ulong aend = start + size;

	while (addr <= (aend - lsize * UNROLL)) {
		cache_unroll(addr, op, lsize);
		addr += lsize * UNROLL;
	}

	while (addr < aend) {
		cache_op(addr, op);
		addr += lsize;
	}
}

static inline_always void __cache_page_op(uint op, uint lsize, ulong page)
{
	ulong addr = page;
	ulong aend = page + PAGE_SIZE;

	do {
		cache_unroll(addr, op, lsize);
		addr += lsize * UNROLL;
	} while (addr < aend);
}

static inline_always void __cache_all_op(uint op, uint lsize, uint sets, uint ways)
{
	ulong base = KSEG0_BASE;		// FIXME: why?
	ulong aend = base + lsize * sets;
	ulong w;

	for (w = 0; w < ways; w++) {
		ulong ws = w * lsize * sets;
		ulong addr;

		for (addr = base; addr < aend; addr += lsize * UNROLL) {
			cache_unroll(addr | ws, op, lsize);
		}
	}
}


#ifdef	CACHE_L2_LINESIZE
#error "L2 cache not yet supported"
#endif

void dcache_clean_range(ulong start, ulong size)
{
	__cache_range_op(CACHEOP_D_HIT_WB, CACHE_LD_LINESIZE, start, size);
}

void dcache_clean_page(ulong page)
{
	__cache_page_op(CACHEOP_D_HIT_WB, CACHE_LD_LINESIZE, page);
}


void dcache_flush_range(ulong start, ulong size)
{
	__cache_range_op(CACHEOP_D_HIT_WBI, CACHE_LD_LINESIZE, start, size);
}

void dcache_flush_page(ulong page)
{
	__cache_page_op(CACHEOP_D_HIT_WBI, CACHE_LD_LINESIZE, page);
}

void dcache_flush_all(void)
{
	__cache_all_op(CACHEOP_D_IDX_WBI, CACHE_LD_LINESIZE, CACHE_LD_SETS, CACHE_LD_WAYS);
}


void dcache_inval_range(ulong start, ulong size)
{
	__cache_range_op(CACHEOP_D_HIT_INV, CACHE_LD_LINESIZE, start, size);
}

void dcache_inval_page(ulong page)
{
	__cache_page_op(CACHEOP_D_HIT_WBI, CACHE_LD_LINESIZE, page);
}

void dcache_inval_all(void)
{
	__cache_all_op(CACHEOP_D_IDX_WBI, CACHE_LD_LINESIZE, CACHE_LD_SETS, CACHE_LD_WAYS);
}
