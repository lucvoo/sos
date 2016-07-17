#include <smp.h>
#include <smp/init.h>
#include <smp/ops.h>
#include <thread.h>


#define	foreach_cpu(cpu)	for (cpu = 0; cpu < NR_CPUS; cpu++)

extern struct thread init_thread[];

void __smp_start_idle_thread(void);
void __smp_start_idle_thread(void)
{
	struct thread *idle = &init_thread[__coreid()];

	asm volatile("\n"
	"	mov	sp, %0\n"
	:: "r" (idle->cpu_context.sp)
	);

	set_current_thread(idle);

	asm volatile("\n"
	"	b	__smp_start\n"
	);
	__builtin_unreachable();
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
		struct thread *idle = &init_thread[cpu];

		if (cpu == 0)
			continue;

		// init thread
		rc = thread_create(idle, 0, NULL, NULL, NULL, 0);
		if (rc) {
			pr_err("cpu %u: can't create idle thread: %d\n", cpu, rc);
			continue;
		}


		// boot the cpu
		// this will call smp_entry()
		// which will do:
		//	* some generic inits
		//	* call smp_secondary_init() for specific inits
		//	* call smp_start()
		rc = smp_ops.boot_cpu(idle, cpu);
		if (rc) {
			pr_err("cpu %u: booting returns %d\n", cpu, rc);
			continue;
		}

		// FIXME: should wait till ??? is finish/released?
	}

	return 0;
}
