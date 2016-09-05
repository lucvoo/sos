#include <smp/ipi.h>
#include <sched.h>
#include <smp.h>


void smp_ipi_resched_cpu(uint cpu)
{
	__smp_ipi_send(cpu, SMP_IPI_RESCHED);
}

void smp_ipi_resched_all(void)
{
	foreach_cpu(i)
		smp_ipi_resched_cpu(i);
}


void __smp_ipi_process(uint ipi)
{
	switch (ipi) {
	case SMP_IPI_RESCHED:
		thread_need_resched_set(get_current_thread());
		break;

	default:
		pr_warn("unknow IPI (%d) received on CPU %d\n", ipi, __cpuid());
	}
}
