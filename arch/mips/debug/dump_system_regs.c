#include <debug.h>
#include <arch/regs-copro.h>


#define	dump__cp0(NAME, REG)					\
	do {	unsigned long r;				\
		asm volatile("mfc0\t%0," #REG : "=r" (r));	\
		printf("%s:\t%08X\n", NAME, r);			\
	} while (0)

#define	dump_cp0(NAME, REG)	dump__cp0(NAME, REG)


void dump_system_regs(void)
{
	printf("\n");
	dump_cp0("c0 status", c0_status);
}
