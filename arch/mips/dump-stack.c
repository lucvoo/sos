#include <exceptions.h>
#include <arch/asm-offsets.h>


void dump_stack(const struct eframe *f, unsigned int flags)
{
	printf("PC %08lx, status %08lx, cause %08lx\n", f->epc, f->status, f->cause);
	printf("r0 %08lx, at %08lx, v0 %08lx, v1 %08lx\n", f->r[0], f->r[1], f->r[2], f->r[3]);
	printf("a0 %08lx, a1 %08lx, a2 %08lx, a3 %08lx\n", f->r[4], f->r[5], f->r[6], f->r[7]);
	printf("t0 %08lx, t1 %08lx, t2 %08lx, t3 %08lx\n", f->r[8], f->r[9], f->r[10], f->r[11]);
	printf("t4 %08lx, t5 %08lx, t6 %08lx, t7 %08lx\n", f->r[12], f->r[13], f->r[14], f->r[15]);
	printf("s0 %08lx, s1 %08lx, s2 %08lx, s3 %08lx\n", f->r[16], f->r[17], f->r[18], f->r[19]);
	printf("s4 %08lx, s5 %08lx, s6 %08lx, s7 %08lx\n", f->r[20], f->r[21], f->r[22], f->r[23]);
	printf("t8 %08lx, t9 %08lx, k0 %08lx, k1 %08lx\n", f->r[24], f->r[25], f->r[26], f->r[27]);
	printf("gp %08lx, sp %08lx, fp %08lx, ra %08lx\n", f->r[28], f->r[29], f->r[30], f->r[31]);
}
