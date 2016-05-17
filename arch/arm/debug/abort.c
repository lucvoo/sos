#include <exceptions.h>
#include <halt.h>
#include <smp.h>


static void abort_handler(struct eframe *regs, ulong far, ulong fsr, const char *type)
{
#ifdef	CONFIG_SMP
#define	F_CPU	" (cpu %u)"
#define	A_CPU	, __coreid()
#else
#define	F_CPU
#define	A_CPU
#endif

	printf("\n%s: FAR=%08lx, FSR=%08lx" F_CPU "\n", type, far, fsr  A_CPU);
	__halt(regs);
}


void pabt_handler(struct eframe *regs, ulong far, ulong fsr)
{
	abort_handler(regs, far, fsr, "PABT");
}

void dabt_handler(struct eframe *regs, ulong far, ulong fsr)
{
	abort_handler(regs, far, fsr, "DABT");
}
