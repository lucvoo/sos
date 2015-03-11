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


void __smp_ipi_process(unsigned long msg)
{
	if (msg & SMP_IPI_SCHEDULE)
		thread_need_resched_set(get_current_thread());

	// FIXME: process the other bits/messages
}
