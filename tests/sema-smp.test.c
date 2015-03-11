#include <kapi/kapi.h>
#include <version.h>
#include <thread.h>
#include <sched.h>
#include <smp/init.h>
#include <smp.h>
#include <semaphore.h>


#define	started(name)	printf("%d: thr0 (%p) started\n", __coreid(), get_current_thread())


static void fun0(void* data)
{
	struct semaphore *sem = data;
	int n = 0;

	started("thr0");
	__smp_init();

	/*
	 * The first wait will block until thr1, on the other cores, is up & running
	 */
	while (1) {
		printf("%d: thr0 run %d\n", __coreid(), ++n);
		printf("%d: thr0 try ...\n", __coreid());
		semaphore_wait(sem);
		printf("%d: thr0 got!\n", __coreid());
		thread_yield();
		printf("%d: thr0 rel\n", __coreid());
		semaphore_post(sem);
	};
}


static struct semaphore sema;
static struct thread thr0 __uninit;

void kapi_start(void)
{

	printf(os_version);

	printf("0: idle = %p\n", get_current_thread());
	semaphore_init(&sema, 0);

	thread_create(&thr0, 2, fun0, &sema, NULL, 0);
	thread_start(&thr0);
}


static void fun1(void *data)
{
	struct semaphore *sem = data;

	started("thr1");

	// Will unblock thr0
	semaphore_post(sem);

	while (1) {
		printf("%d: thr1 sleep ...\n", __coreid());
		thread_schedule_timeout(1 << 13);
		printf("%d: thr1 awake!\n", __coreid());
		printf("%d: thr1 try ...\n", __coreid());
		semaphore_wait(sem);
		printf("%d: thr1 got!\n", __coreid());
		printf("%d: thr1 rel\n", __coreid());
		semaphore_post(sem);
	}
}

static void fun2(void *data)
{
	struct semaphore *sem = data;

	started("thr2");

	while (1) {
		printf("%d: thr2 try ...\n", __coreid());
		semaphore_wait(sem);
		printf("%d: thr2 got!\n", __coreid());
		printf("%d: thr2 sleep ...\n", __coreid());
		thread_schedule_timeout(3 << 11);
		printf("%d: thr2 awake!\n", __coreid());
		printf("%d: thr2 rel\n", __coreid());
		semaphore_post(sem);
	}
}

static void fun3(void *data)
{
	started("thr3");

	while (1) {
		//printf("%d: thr3 running\n", __coreid());
		thread_yield();
	}
}

static void fun4(void *data)
{
	started("thr4");

	while (1) {
		//printf("%d: thr4 running\n", __coreid());
		thread_yield();
	}
}

static struct thread thr1 __uninit;
static struct thread thr2 __uninit;
static struct thread thr3 __uninit;
static struct thread thr4 __uninit;

void kapi_start_smp(void)
{
	printf("%d: SMP started\n", __coreid());
	printf("%d: idle = %p\n", __coreid(), get_current_thread());

	thread_create(&thr1, 2, fun1, &sema, NULL, 0);
	thread_create(&thr2, 2, fun2, &sema, NULL, 0);
	thread_create(&thr3, 2, fun3, &sema, NULL, 0);
	thread_create(&thr4, 2, fun4, &sema, NULL, 0);
	thread_start(&thr2);
	thread_start(&thr1);
	thread_start(&thr3);
	thread_start(&thr4);
}
