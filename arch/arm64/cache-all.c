#include <bitops/findbit.h>
#include <arch/cacheflush.h>
#include <arch/barrier.h>
#include <arch/arch.h>
#include <arch/msr.h>
#include <soc/cache.h>


#define	dc(op, val)	asm volatile("dc " #op ", %0" :: "r" (val));

#if defined(CACHE_L2_SETS)
static void dcache_inval_all_level(uint level, uint ll, uint lw, uint ls)
{
	uint val;
	uint os;	// bit offset for set (L in refman)
	uint ow;	// bit offset for ways (A in refman)
	uint nways = 1 << lw;
	uint nsets = 1 << ls;

	os = ll;
	ow = 32 - lw;

	val = level << 1;
	val |= nways << ow;
	do {
		val |= nsets << os;
		val -= 1 << ow;

		do {
			val -= 1 << os;
			dc(isw, val);
		} while (val & MASK(os, ls));
	} while (val & MASK(ow, lw));

	dsb(sy);
}

void dcache_inval_all(void)
{
	dcache_inval_all_level(2, CACHE_L2_SIZ_ORDER, CACHE_L2_WAY_ORDER, CACHE_L2_SET_ORDER);
	dcache_inval_all_level(1, CACHE_LD_SIZ_ORDER, CACHE_LD_WAY_ORDER, CACHE_LD_SET_ORDER);
}
#else
static void dcache_inval_all_level(uint level)
{
	uint val;
	uint ccs;
	uint os;	// bit offset for set (L in refman)
	uint ow;	// bit offset for ways (A in refman)
	uint nsets;	// number of sets minus 1
	uint nways;	// number of ways
	uint uw, us;

	// select the cache level
	msr_setval(csselr_el1, BITS_MAKEVAL(CSSELR_LEVEL, level - 1));
	isb();

	ccs = msr_getval(ccsidr_el1);

	os = BITS_EXTRACT(ccs, CCSIDR_LINESIZE) + 4;
	us = 1 << os;

	nways = BITS_EXTRACT(ccs, CCSIDR_NWAYS) + 1;
	ow = 32 - bitop_log2(nways);
	uw = 1 << ow;

	nsets = BITS_EXTRACT(ccs, CCSIDR_NSETS);
	val = BITS_MAKEVAL(DCISW_LEVEL, level);
	val |= nways << ow;
	do {
		int set = nsets << os;

		val -= uw;
		do {
			dc(isw, val | set);
			set -= us;
		} while (set >= 0);

	} while (val > BITS_MASK(DCISW_LEVEL));

	dsb(sy);
}

void dcache_inval_all(void)
{
	uint levels;
	uint loc;
	uint l;

	levels = msr_getval(clidr_el1);
	loc = BITS_EXTRACT(levels, CLIDR_LoC);

	for (l = loc; l != 0; l--) {
		uint type;

		type = BITS_EXTRACT(levels, CLIDR_CTYPE(l));
		if (type < CLIDR_CTYPE_DCACHE)
			continue;

		dcache_inval_all_level(l);
	}
}
#endif
