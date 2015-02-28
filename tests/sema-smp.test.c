#include <kapi/kapi.h>
#include <version.h>
#include <thread.h>
#include <sched.h>
#include <smp.h>
#include <semaphore.h>


static void fun0(void* data)
{
	struct semaphore *sem = data;
	const char *name = "thr0";
	int n = 0;

	printf("%s started ...\n", name);
	thread_yield();
	__smp_init();

	while (1) {
		printf("%s try %d ...\n", name, ++n);
		semaphore_wait(sem);
		printf("%s got!\n", name);
		thread_yield();
		printf("%s rel semaphore\n", name);
		semaphore_post(sem);
		printf("%s pos\n", name);
	};
}


static struct semaphore sema;
static struct thread thr0 __uninit;

void kapi_start(void)
{

	printf(os_version);

	semaphore_init(&sema, 1);

	thread_create(&thr0, 2, fun0, &sema, NULL, 0);
	thread_start(&thr0);
}


static void fun1(void *data)
{
	printf("thr1 started\n");

	while (1) {
		thread_yield();
		printf("thr1 running\n");
	}
}

static void fun2(void *data)
{
	printf("thr2 started\n");

	while (1) {
		thread_yield();
		printf("thr2 running\n");
	}
}

static struct thread thr1 __uninit;
static struct thread thr2 __uninit;

void kapi_start_smp(void)
{
	printf("SMP started\n");

	thread_create(&thr1, 2, fun1, &sema, NULL, 0);
	thread_create(&thr2, 2, fun2, &sema, NULL, 0);
	thread_start(&thr2);

	thread_start(&thr1);
}
