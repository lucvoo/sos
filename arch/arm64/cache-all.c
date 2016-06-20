#include <bitops/findbit.h>
#include <arch/cacheflush.h>
#include <arch/barrier.h>
#include <arch/arch.h>
#include <arch/msr.h>


#define	dc(op, val)	asm volatile("dc " #op ", %0" :: "r" (val));

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
