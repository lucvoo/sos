#include <kapi/kapi.h>
#include <thread.h>
#include <sched.h>
#include <slab.h>


struct ptr_array {
	struct kmem_cache*	slab;
	unsigned int		nbr;
	void*			ptr[4096];
};

static void fun(void* data)
{
	struct ptr_array *arr = data;
	int n = 1 << 12;

	do {
		unsigned int i;

		for (i = arr->nbr; i--; ) {
			void *ptr;

			ptr = kmem_alloc(arr->slab);
			arr->ptr[i] = ptr;
		}
		thread_yield();

		for (i = arr->nbr; i--; ) {
			kmem_free(arr->slab, arr->ptr[i]);
		}
		thread_yield();
	} while (n--);

	printf("test %d: finished\n", arr->nbr);
}


static struct thread	thr1 __uninit;
static struct ptr_array	arr1;

static struct thread	thr2 __uninit;
static struct ptr_array	arr2;

void kapi_start(void)
{
	struct kmem_cache* slab;

	slab = kmem_cache_create("test", 50, 0, 0);

	arr1.slab = slab;
	arr1.nbr  = 1024;
	thread_create(&thr1, 2, fun, &arr1, NULL, 0);
	thread_start(&thr1);

	arr2.slab = slab;
	arr2.nbr  = 4096;
	thread_create(&thr2, 2, fun, &arr2, NULL, 0);
	thread_start(&thr2);
}
