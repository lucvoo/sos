#include <kapi/kapi.h>
#include <version.h>
#include <thread.h>
#include <sched.h>
#include "tests-utils.h"
#include <kmalloc.h>


static struct thread t __uninit;

#define	MAX_ALLOCS	(1 << 16)
#define	NBR_RUNS	64

static struct item {
	void		*buf;
	unsigned int	size;
} items[MAX_ALLOCS];


static unsigned long free_idx(unsigned int i)
{
	struct item *p = &items[i];
	uint size = p->size;

	kfree(p->buf);

	return size;
}

static unsigned long free_all_head(unsigned int max)
{
	unsigned long n = 0;
	unsigned int i;

	for (i = 0; i < max; i++) {
		n += free_idx(i);
	}

	pr_dbg("free %lu bytes\n", n);
	return n;
}

static unsigned long free_all_tail(unsigned int max)
{
	unsigned long n = 0;

	while (max--) {
		n += free_idx(max);
	}

	pr_dbg("free %lu bytes\n", n);
	return n;
}

static unsigned long free_rnd(unsigned int max)
{
	unsigned int i = sprng() % max;
	unsigned long n;

	n = free_idx(i);
	items[i] = items[max-1];

	return n;
}

static unsigned long free_all_rnd(unsigned int max)
{
	unsigned long n = 0;

	while (max) {
		n += free_rnd(max--);
	}

	pr_dbg("free %lu bytes\n", n);
	return n;
}

static int test_alloc_free_all(const char *name, unsigned long (*free_all)(unsigned int max))
{
	unsigned long n = 0;
	unsigned int i = 0;
	unsigned long f;

	pr_info("\ntesting %s\n", name);
	while (i < MAX_ALLOCS) {
		unsigned int size;
		void *p;

		size = ((sprng() & 0x7f) + 1) >> 2;
		p = kmalloc(size, 0);
		if (p) {
			n += size;
			items[i++] = ((struct item) { p, size, });
		}

		pr_dbg("(%p, %03x), tot = %x\n", p, size, n);

		if (!p)
			break;
	}

	pr_info("nbr alloc = %u, total = %lu (%lu)\n\n", i, n, n + i*4);

	f = free_all(i);
	if (f != n) {
		pr_err("%s: f = %lu but n was %lu\n", name, f, n);
		return 1;
	}

	return 0;
}

#define RESULT(R, F)	if (R(#F, F)) ko++; else ok++

static void fun(void* data)
{
	unsigned int ok = 0;
	unsigned int ko = 0;
	int i;

	for (i = 0; i < NBR_RUNS; i++) {
		RESULT(test_alloc_free_all, free_all_tail);
		RESULT(test_alloc_free_all, free_all_head);
		RESULT(test_alloc_free_all, free_all_rnd);
	}

	printf("TEST kmalloc: fail = %u, succeed = %i\n", ko, ok);
	thread_sleep();
}

void kapi_start(void)
{
	printf(os_version);

	thread_create(&t, 2, fun, NULL, NULL, 0);
	thread_start(&t);
}
