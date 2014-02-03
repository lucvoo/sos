#define	BYTE(V, N)	(((V) >> ((N)*8)) & 0xFF)

#define	dump_reg(NAME, ASM)	\
	do { unsigned long r; __asm__ __volatile__(ASM : "=r" (r)); printf("%s:\t%08X (%08b %08b %08b %08b)\n", NAME, r, BYTE(r,3), BYTE(r,2), BYTE(r,1), BYTE(r,0)); } while (0)


void dump_system_regs(void)
{
	printf("\n");
	dump_reg("MIDR",  "mrc   p15, 0, %0, c0, c0, 0");
	dump_reg("TCMTR", "mrc   p15, 0, %0, c0, c0, 2");
	dump_reg("TLBTR", "mrc   p15, 0, %0, c0, c0, 3");
	dump_reg("MPIDR", "mrc   p15, 0, %0, c0, c0, 5");
	dump_reg("REVIDR","mrc   p15, 0, %0, c0, c0, 6");

	printf("\n");
	dump_reg("CTR",   "mrc   p15, 0, %0, c0, c0, 1");
	dump_reg("CCSIDR","mrc   p15, 1, %0, c0, c0, 1");
	dump_reg("CLIDR", "mrc   p15, 1, %0, c0, c0, 7");

	printf("\n");
	dump_reg("CPSR",  "mrs %0, cpsr");
	dump_reg("SCTRL", "mrc   p15, 0, %0, c1, c0, 0");
	dump_reg("VBAR",  "mrc   p15, 0, %0, c12, c0, 0");

	printf("\n");
	dump_reg("TTBCR", "mrc   p15, 0, %0, c2, c0, 2");
	dump_reg("TTBR0", "mrc   p15, 0, %0, c2, c0, 0");
	dump_reg("TTBR1", "mrc   p15, 0, %0, c2, c0, 1");
	dump_reg("PRRR",  "mrc   p15, 0, %0, c10, c2, 0");
	dump_reg("NMRR",  "mrc   p15, 0, %0, c10, c2, 1");

	printf("\n");
	dump_reg("NSACR", "mrc   p15, 0, %0, c1, c1, 2");
	//dump_reg("SCR",   "mrc   p15, 0, %0, c1, c1, 0");


#if 0
	printf("\n");
	dump_reg("CNTFRQ","p15, 0, %0, c14, c0, 0");
#endif
}
