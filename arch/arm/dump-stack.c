#include <exceptions.h>
#include <arch/asm-offsets.h>


void dump_stack(const struct eframe *f, unsigned int flags)
{
	printf("PC  %08x, LR  %08x, SP  %08x, FP  %08x, PSR %08x\n", f->pc, f->lr, f->sp, f->r[11], f->cpsr);
	printf("R0  %08x, R1  %08x, R2  %08x, R3  %08x\n", f->r[0], f->r[1], f->r[2], f->r[3]);
	printf("R4  %08x, R5  %08x, R6  %08x, R7  %08x\n", f->r[4], f->r[5], f->r[6], f->r[7]);
	printf("R8  %08x, R9  %08x, R10 %08x, R12 %08x\n", f->r[8], f->r[9], f->r[10], f->r[12]);
}
