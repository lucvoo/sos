#include <kapi/kapi.h>
#include <version.h>
#include <thread.h>
#include <sched.h>
#include <smp/init.h>
#include <smp.h>
#include <semaphore.h>
#include <delay.h>
#include <hz.h>


#define	started(name)	printf("%d: %s (%p) started\n", __cpuid(), name, get_current_thread())




static void do_loop_wait(struct semaphore *sem, const char *name, uint delay)
{
	while (1) {
		printf("%d: %s sleep ...\n", __cpuid(), name);
		thread_schedule_timeout(delay);
		printf("%d: %s awake!\n", __cpuid(), name);

		printf("%d: %s try ...\n", __cpuid(), name);
		semaphore_wait(sem);
		printf("%d: %s got!\n", __cpuid(), name);

		mdelay((delay * 1024)/ HZ);

		printf("%d: %s rel\n", __cpuid(), name);
		semaphore_post(sem);
	}
}

static void fun1(void *data)
{
	struct semaphore *sem = data;
	const char *name = "thr1";

	started(name);

	// Will unblock thr0
	printf("%d: %s rel\n", __cpuid(), name);
	semaphore_post(sem);

	do_loop_wait(sem, name, HZ/4);
}

static void fun2(void *data)
{
	struct semaphore *sem = data;
	const char *name = "thr2";

	started(name);

	do_loop_wait(sem, name, 3*HZ/16);
}


static void do_loop_yield(const char *name)
{
	started(name);

	while (1) {
		//printf("%d: %s running\n", __cpuid(), name);
		thread_yield();
	}
}

/******************************************************************************/
static struct thread thr1 __uninit;
static struct thread thr2 __uninit;

static void fun0(void* data)
{
	struct semaphore *sem = data;
	const char *name = "thr0";
	int n = 0;

	started(name);

	__smp_init();
	thread_create(&thr1, 2, fun1, sem, NULL, 0);
	thread_create(&thr2, 2, fun2, sem, NULL, 0);
	thread_start(&thr2);
	thread_start(&thr1);

	/*
	 * The first wait will block until thr1, on the other cores, is up & running
	 */
	while (1) {
		printf("%d: %s try %d ...\n", __cpuid(), name, ++n);
		semaphore_wait(sem);
		printf("%d: %s got!\n", __cpuid(), name);

		mdelay(50);

		printf("%d: %s rel\n", __cpuid(), name);
		semaphore_post(sem);
	};
}


static struct semaphore sema;
static struct thread thr0 __uninit;

void kapi_start(void)
{
	printf("%d: started (idle = %p)\n", __cpuid(), get_current_thread());

	semaphore_init(&sema, 0);

	thread_create(&thr0, 2, fun0, &sema, NULL, 0);
	thread_start(&thr0);
}

void kapi_start_smp(void)
{
	printf("%d: started (idle = %p)\n", __cpuid(), get_current_thread());
}
