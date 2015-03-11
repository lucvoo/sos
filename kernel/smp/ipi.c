#include <smp/ipi.h>
#include <sched.h>
#include <smp.h>


void smp_ipi_schedule_one(unsigned int cpu)
{
	__smp_ipi_send(cpu, SMP_IPI_SCHEDULE);
}

void smp_ipi_schedule_all(void)
{
	unsigned int i;

	for (i = 0; i < NR_CPUS; i++)
		smp_ipi_schedule_one(i);
}
