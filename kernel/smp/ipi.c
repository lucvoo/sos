#include <smp/ipi.h>
#include <sched.h>
#include <smp.h>


void smp_ipi_schedule_one(unsigned int cpu)
{
	__smp_ipi_send(cpu, SMP_IPI_SCHEDULE);
}

void smp_ipi_schedule_all(void)
{
	foreach_cpu(i)
		smp_ipi_schedule_one(i);
}


void __smp_ipi_process(uint ipi)
{
	switch (ipi) {
	case SMP_IPI_SCHEDULE:
		thread_need_resched_set(get_current_thread());
		break;

	default:
		pr_warn("unknow IPI (%d) received on CPU %d\n", ipi, __cpuid());
	}
}
