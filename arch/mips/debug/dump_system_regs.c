#include <debug.h>
#include <arch/copro.h>
#include <arch/memory.h>
#include <soc/regs-copro.h>


#define	dump_reg(INS, NAME, ARG)				\
	do {							\
		asm volatile(INS "\t%0," ARG : "=r" (r));	\
		printf("%s:\t%08X (%#035b)\n", NAME, r, r);	\
	} while (0)

#define	dump_cp0(REG)		dump_reg("mfc0", #REG, REG)

#define	dump_mmio(BASE, OFF)					\
	do {							\
		unsigned long r;				\
		r = ioread32(KSEG1_BASE + BASE + OFF);		\
		printf(#OFF ":\t%08x (%#035b)\n", r, r);	\
	} while (0)


#include <soc/dump-regs.h>

#define	CONFIG_M	(1 << 31)

void dump_system_regs(void)
{
	ulong r;

	printf("\n");
	dump_cp0(c0_status);
	dump_cp0(c0_cause);
	dump_cp0(c0_intctl);
	dump_cp0(c0_hwrena);
	dump_cp0(c0_count);
	dump_cp0(c0_prid);
	dump_cp0(c0_srsctl);
	dump_cp0(c0_srsmap);
	dump_cp0(c0_ebase);
	dump_cp0(c0_config);
	if (r & CONFIG_M)
		dump_cp0(c0_config1);
	if (r & CONFIG_M)
		dump_cp0(c0_config2);
	if (r & CONFIG_M)
		dump_cp0(c0_config3);
	if (r & CONFIG_M)
		dump_cp0(c0_config4);
	if (r & CONFIG_M)
		dump_cp0(c0_config5);

#if defined(CONFIG_MT)
	printf("\n");
	dump_cp0(c0_mvpctrl);
	dump_cp0(c0_mvpcfg0);
	if (r & CONFIG_M)
		dump_cp0(c0_mvpcfg1);

	dump_cp0(c0_vpectrl);
	dump_cp0(c0_vpecfg0);
	if (r & CONFIG_M)
		dump_cp0(c0_vpecfg1);
	dump_cp0(c0_vpesched);
	dump_cp0(c0_vpeschedfb);
	dump_cp0(c0_vpeopt);

	dump_cp0(c0_srscfg0);
	if (r & CONFIG_M)
		dump_cp0(c0_srscfg1);
	if (r & CONFIG_M)
		dump_cp0(c0_srscfg2);
	if (r & CONFIG_M)
		dump_cp0(c0_srscfg3);
	if (r & CONFIG_M)
		dump_cp0(c0_srscfg4);
#endif

	dump_regs_soc();
}
