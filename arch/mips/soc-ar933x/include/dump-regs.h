#include <io.h>



static inline void dump_regs_soc(void)
{
	ulong r;

	printf("-- SOC --\n");
	dump_cp0(c0_config7);
}
