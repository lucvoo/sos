#include <debug.h>
#include <arch/cp15.h>
#include <arch/irqflags.h>
#include <stringify.h>
#include <lock.h>


extern struct lock printf_lock;
#define	printf __printf

#define	BYTE(V, N)	(((V) >> ((N)*8)) & 0xFF)

#define	dump_reg(NAME, ASM)	\
	do { unsigned long r; __asm__ __volatile__(ASM : "=r" (r)); printf("%s:\t%08X (%08b %08b %08b %08b)\n", NAME, r, BYTE(r,3), BYTE(r,2), BYTE(r,1), BYTE(r,0)); } while (0)
#define	dump_cp15(NAME)	dump_reg(#NAME, "mrc " STRINGIFY(NAME(%0)))


void dump_system_regs(void)
{
	ulong flags = lock_acq_save(&printf_lock);

	printf("\n");
	dump_cp15(MIDR);
	dump_cp15(TCMTR);
	dump_cp15(TLBTR);
	dump_cp15(MPIDR);
	dump_cp15(REVIDR);

	printf("\n");
	dump_cp15(CTR);
#if CONFIG_ARM_ARCH >= 7
	dump_cp15(CCSIDR);
	dump_cp15(CLIDR);
#endif

	printf("\n");
	dump_reg("CPSR",  __irqflags_save("%0"));
	dump_cp15(SCTLR);
#ifdef CONFIG_HAS_ACTLR
	dump_cp15(ACTLR);
#endif
	dump_cp15(VBAR);

	printf("\n");
	dump_cp15(TTBCR);
	dump_cp15(TTBR0);
	dump_cp15(TTBR1);
	dump_cp15(PRRR);
	dump_cp15(NMRR);

	printf("\n");
	dump_cp15(NSACR);
	//dump_cp15(SCR);


#ifdef	CONFIG_GENERIC_TIMER
	printf("\n");
	dump_cp15(CNTFRQ);
#endif

#ifdef CONFIG_ARM_GIC
	dump_cp15(CBAR);
#endif
	lock_rel_rest(&printf_lock, flags);
}
