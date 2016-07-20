#include <kapi/kapi.h>
#include <smp/init.h>
#include <smp/ops.h>
#include <smp.h>
#include <symbols.h>
#include <idle.h>
#include <sched.h>


#define	foreach_cpu(cpu)	for (cpu = 0; cpu < NR_CPUS; cpu++)

extern struct thread init_thread[];

int __weak __smp_init(void)
{
	uint cpu;
	int rc;

	rc = smp_ops.init();
	if (rc) {
		pr_err("mach smp init failed (%d)\n", rc);
		return rc;
	}

	foreach_cpu(cpu) {
		struct thread *idle = &init_thread[cpu];

		if (cpu == 0)
			continue;

		// init thread
		rc = thread_create(idle, 0, NULL, NULL, NULL, 0);
		if (rc) {
			pr_err("cpu %u: can't create idle thread: %d\n", cpu, rc);
			continue;
		}

		// boot the cpu:
		rc = smp_ops.boot_cpu(idle, cpu);
		if (rc) {
			pr_err("cpu %u: booting returns %d\n", cpu, rc);
			continue;
		}

		// FIXME: should wait till cpu is started?
	}

	return 0;
}


// This function MUST NOT use the stack, ever: it hasn't one yet.
struct thread *__smp_init_idle_thread(void);		// called from asm
struct thread *__smp_init_idle_thread(void)
{
	struct thread *idle = &init_thread[__coreid()];

	set_current_thread(idle);

	return idle;
}

void __smp_start(void)
{
	uint cpu = __coreid();

	if (smp_ops.init_cpu)
		smp_ops.init_cpu(cpu);

	kapi_start_smp();
	_thread_scheduler_start();
	__cpu_idle();
}


void __weak kapi_start_smp(void)
{
	// Do nothing
}
