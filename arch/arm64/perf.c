#include <perf.h>
#include <init.h>


#define PMCR_LC		(1 << 6)
#define PMCR_DP		(1 << 5)
#define PMCR_X		(1 << 4)
#define PMCR_D		(1 << 3)
#define PMCR_C		(1 << 2)
#define PMCR_P		(1 << 1)
#define PMCR_E		(1 << 0)

#define PMCNTEN_C	(1 << 31)


static void perf_counters_start(uint mask)
{
	asm volatile("msr	pmcntenset_el0, %0" :: "r" (mask));
}

static void perf_counters_stop(uint mask)
{
	asm volatile("msr	pmcntenclr_el0, %0" :: "r" (mask));
}

static void pmcr_set(uint bits)
{
	ulong val;

	asm volatile("mrs	%0, pmcr_el0" : "=r" (val));
	val |= bits;
	asm volatile("msr	pmcr_el0, %0" :: "r" (val));
}


void perf_cycle_counter_start(void)
{
	perf_counters_start(PMCNTEN_C);
}

void perf_cycle_counter_stop(void)
{
	perf_counters_stop(PMCNTEN_C);
}

void perf_cycle_counter_reset(void)
{
	pmcr_set(PMCR_C);
}

ulong perf_cycle_counter_get(void)
{
	ulong val;

	asm volatile("mrs %0, pmccntr_el0" : "=r" (val));

	return val;
}


static void perf_v7_init(void)
{
	perf_counters_stop(0xFFFFFFFF);		// Insure no counter is running
	pmcr_set(PMCR_E);			// global enable
}
core_initcall(perf_v7_init);
