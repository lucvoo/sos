#include <delay.h>
#include <barrier.h>
#include <limits.h>
#include <hz.h>


// remove common power of N from A & B
#define	reduce(A, B, N)				\
	if (((A % N) == 0) && ((B % N) == 0)) {	\
		A /= N;				\
		B /= N;				\
	}
#define reduce_by_1000(A, B)	\
	reduce(A, B, 10); reduce(A, B, 10); reduce(A, B, 10)


#ifdef CONFIG_LPUSEC_FIXED
static void delay_loops(unsigned int loops)
{
	volatile unsigned int l = loops;

	while (--l)
		;
}
static void __udelay(unsigned int usec)
{
	unsigned int mult = CONFIG_LPUSEC_MULT;
	unsigned int shift = CONFIG_LPUSEC_SHIFT;
	unsigned int nloops;

	nloops = (usec * mult + (1 << shift) - 1) >> shift;

	delay_loops(nloops);
}
#else
#include <timerdev.h>

static inline uint div_roundup(uint n, uint mul, uint div)
{
	reduce_by_1000(mul, div);
	reduce_by_1000(mul, div);

	return (n * mul + div - 1) / div;
}

static void __udelay(uint usecs)
{
	uint ticks = div_roundup(usecs, HZ, 1000000);

	ticks += timerdev_read();

	while (((int) (ticks - timerdev_read())) > 0) {
		barrier();
	}
}
#endif


void udelay(unsigned int usec)
{
	uint div = 1000000;
	uint mul = HZ;
	uint max;

	reduce_by_1000(mul, div);
	reduce_by_1000(mul, div);
	max = UINT_MAX / mul;

	while (usec >= max) {
		__udelay(max);
		usec -= max;
	}

	__udelay(usec);
}

void mdelay(unsigned int msec)
{
	udelay(msec * 1000);
}
