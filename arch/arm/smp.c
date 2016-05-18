#include <smp.h>
#include <smp/init.h>
#include <smp/ops.h>
#include <thread.h>


#define	foreach_cpu(cpu)	for (cpu = 0; cpu < NR_CPUS; cpu++)

// FIXME: should allocate this dynamically
static struct thread idle_threads[NR_CPUS-1];

static int smp_boot_cpu(uint cpu)
{
	struct thread *idle = &idle_threads[cpu-1];
	int rc;

	// initialize the thread with the default stack size
	thread_create(idle, 0, NULL, NULL, NULL, 0);


	rc = smp_ops.boot_cpu(idle, cpu);
	return rc;
}

int __smp_init(void)
{
	uint cpu;
	int rc;

	rc = smp_ops.init();
	if (rc) {
		pr_err("mach smp init failed (%d)\n", rc);
		return rc;
	}

	foreach_cpu(cpu) {
		if (cpu == 0)
			continue;

		// allocate thread & stack


		// boot the cpu
		// this will call smp_entry()
		// which will do:
		//	* some generic inits
		//	* call smp_secondary_init() for specific inits
		//	* call smp_start()
		rc = smp_boot_cpu(cpu);
		if (rc) {
			pr_err("cpu %u: booting returns %d\n", cpu, rc);
			continue;
		}

		// FIXME: should wait till ??? is finish/released?
	}

	return 0;
}
