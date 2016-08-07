#include <kapi/kapi.h>
#include <version.h>
#include <thread.h>
#include <sched.h>
#include <smp/init.h>
#include <smp.h>
#include <semaphore.h>
#include <delay.h>
#include <hz.h>


#define	started(name)	printf("%d: %s (%p) started\n", __coreid(), name, get_current_thread())




static void do_loop_wait(struct semaphore *sem, const char *name)
{
	started(name);

	while (1) {
		printf("%d: %s try ...\n", __coreid(), name);
		semaphore_wait(sem);
		printf("%d: %s got!\n", __coreid(), name);
	}
}


static void fun1(void *data)
{
	struct semaphore *sem = data;
	const char *name = "thr1";

	do_loop_wait(sem, name);
}

static void fun2(void *data)
{
	struct semaphore *sem = data;
	const char *name = "thr2";

	do_loop_wait(sem, name);
}


static void do_loop_post(struct semaphore *sem, const char *name)
{
	int n = 0;

	while (1) {
		printf("%d: %s sleep ... %d\n", __coreid(), name, ++n);
		mdelay(150);
		printf("%d: %s awake!\n", __coreid(), name);

		printf("%d: %s rel\n", __coreid(), name);
		semaphore_post(sem);
	}
}

static void fun0(void* data)
{
	struct semaphore *sem = data;
	const char *name = "thr0";

	started(name);

	__smp_init();

	do_loop_post(sem, name);
}

/******************************************************************************/
static struct thread thr0 __uninit;
static struct thread thr1 __uninit;
static struct thread thr2 __uninit;

static struct semaphore sema;


void kapi_start(void)
{
	printf("%d: started (idle = %p)\n", __coreid(), get_current_thread());

	semaphore_init(&sema, 0);

	thread_create(&thr0, 2, fun0, &sema, NULL, 0);
	thread_start(&thr0);

	thread_create(&thr1, 2, fun1, &sema, NULL, 0);
	thread_create(&thr2, 2, fun2, &sema, NULL, 0);
	thread_start(&thr2);
	thread_start(&thr1);
}

void kapi_start_smp(void)
{
	printf("%d: started (idle = %p)\n", __coreid(), get_current_thread());
}
