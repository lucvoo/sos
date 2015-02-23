#include <debug.h>
#include <arch/regs-copro.h>


#define	dump_reg(INS, NAME, ARG)				\
	do {	unsigned long r;				\
		asm volatile(INS "\t%0," ARG : "=r" (r));	\
		printf("%s:\t%08X (%#035b)\n", NAME, r, r);	\
	} while (0)

#define	dump_cp0(REG)		dump_reg("mfc0", #REG, REG)


void dump_system_regs(void)
{
	printf("\n");
	dump_cp0(c0_status);
	dump_cp0(c0_cause);
	dump_cp0(c0_intctl);
	dump_cp0(c0_hwrena);
	dump_cp0(c0_count);
	dump_cp0(c0_prid);
	dump_cp0(c0_ebase);
	dump_cp0(c0_config);
	dump_cp0(c0_config1);
	dump_cp0(c0_config2);
	dump_cp0(c0_config3);
}
