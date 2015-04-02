#include <delay.h>


static void delay_loops(unsigned int loops)
{
	volatile unsigned int l = loops;

	while (--l)
		;
}

#ifdef CONFIG_LPUSEC_FIXED
static void __udelay(unsigned int usec)
{
	unsigned int mult = CONFIG_LPUSEC_MULT;
	unsigned int shift = CONFIG_LPUSEC_SHIFT;
	unsigned int nloops;

	nloops = (usec * mult + (1 << shift) - 1) >> shift;

	delay_loops(nloops);
}
#endif


void udelay(unsigned int usec)
{
	while (usec > 1024) {
		__udelay(1024);
		usec -= 1024;
	}

	__udelay(usec);
}
