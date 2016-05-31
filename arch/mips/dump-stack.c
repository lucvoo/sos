#include <exceptions.h>
#include <arch/asm-offsets.h>

#ifdef	CONFIG_SMP
#include <soc/smp.h>
#else
#define	__coreid()	0
#endif


void __arch_dump_stack(const struct eframe *f)
{
#define	printf	__printf

	printf("PC %08lx  status %08lx cause %08lx core %d\n"
	       "r0 %08lx, at %08lx, v0 %08lx, v1 %08lx\n"
	       "a0 %08lx, a1 %08lx, a2 %08lx, a3 %08lx\n"
	       "t0 %08lx, t1 %08lx, t2 %08lx, t3 %08lx\n"
	       "t4 %08lx, t5 %08lx, t6 %08lx, t7 %08lx\n"
	       "s0 %08lx, s1 %08lx, s2 %08lx, s3 %08lx\n"
	       "s4 %08lx, s5 %08lx, s6 %08lx, s7 %08lx\n"
	       "t8 %08lx, t9 %08lx, k0 %08lx, k1 %08lx\n"
	       "gp %08lx, sp %08lx, fp %08lx, ra %08lx\n"
		, f->epc, f->status, f->cause, __coreid()
		, f->r[0], f->r[1], f->r[2], f->r[3]
		, f->r[4], f->r[5], f->r[6], f->r[7]
		, f->r[8], f->r[9], f->r[10], f->r[11]
		, f->r[12], f->r[13], f->r[14], f->r[15]
		, f->r[16], f->r[17], f->r[18], f->r[19]
		, f->r[20], f->r[21], f->r[22], f->r[23]
		, f->r[24], f->r[25], f->r[26], f->r[27]
		, f->r[28], f->r[29], f->r[30], f->r[31]);
}
