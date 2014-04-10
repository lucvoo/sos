#include <perf.h>
#include <arch/cp15.h>
#include <stringify.h>
#include <init.h>


#define PMCR_DP		(1 << 5)
#define PMCR_X		(1 << 4)
#define PMCR_D		(1 << 3)
#define PMCR_C		(1 << 2)
#define PMCR_P		(1 << 1)
#define PMCR_E		(1 << 0)

#define CNT_IDX_CYCLE	31


static void perf_counters_start(unsigned int mask)
{
	asm volatile(
		"mcr " STRINGIFY(PMCNTENSET(%0))
		:
		: "r" (mask)
		: );
}

static void perf_counters_stop(unsigned int mask)
{
	asm volatile(
		"mcr " STRINGIFY(PMCNTENCLR(%0))
		:
		: "r" (mask)
		: );
}

void perf_cycle_counter_start(void)
{
	perf_counters_start(1 << CNT_IDX_CYCLE);
}

void perf_cycle_counter_stop(void)
{
	perf_counters_stop(1 << CNT_IDX_CYCLE);
}

void perf_cycle_counter_reset(void)
{
	unsigned long val;

	asm volatile(
		"mrc " STRINGIFY(PMCR(%0)) "\n"
		"orr %0, %0, %1\n"
		"mcr " STRINGIFY(PMCR(%0)) "\n"
		: "=r" (val)
		: "i" (PMCR_C)
		: );
}

unsigned long perf_cycle_counter_get(void)
{
	unsigned long val;

	asm volatile(
		"mrc " STRINGIFY(PMCCNTR(%0))
		: "=r" (val)
		:
		: );

	return val;
}


static void perf_v7_init(void)
{
	unsigned long val;

	perf_counters_stop(0xFFFFFFFF);			// Insure no counter is running

	asm volatile(
		"mrc " STRINGIFY(PMCR(%0)) "\n"		// Enable the counters as a whole
		"orr %0, %0, %1\n"			//
		"mcr " STRINGIFY(PMCR(%0))		//
		: "=r" (val)
		: "i" (PMCR_E)
		: );
}
core_initcall(perf_v7_init);
