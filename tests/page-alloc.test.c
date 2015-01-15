#include <kapi/kapi.h>
#include <version.h>
#include <thread.h>
#include <sched.h>
#include "tests-utils.h"
#include <page.h>
#include <page-alloc.h>
#include <memory.h>
#include <debug.h>


static struct thread t __uninit;

#define	MAX_ALLOCS	65536

static struct pages {
	struct page *p;
	unsigned int o;
} pages[MAX_ALLOCS];


static unsigned long free_idx(unsigned int i)
{
	struct pages *p = &pages[i];
	unsigned int order = p->o;

	page_free(p->p, order);

	dbg("pf(%2u): pfn=%lx\n", order, page_to_pfn(p->p));
	return 1 << order;
}

static unsigned long free_all_head(unsigned int max)
{
	unsigned long n = 0;
	unsigned int i;

	for (i = 0; i < max; i++) {
		n += free_idx(i);
	}

	dbg("%s(): free %lu pages\n", __func__, n);
	return n;
}

static unsigned long free_all_tail(unsigned int max)
{
	unsigned long n = 0;

	while (max--) {
		n += free_idx(max);
	}

	dbg("%s(): free %lu pages\n", __func__, n);
	return n;
}

static int test_alloc_free_all(const char *name, unsigned long (*free_all)(unsigned int max))
{
	unsigned long n = 0;
	unsigned int i = 0;
	unsigned long f;

	while (1) {
		unsigned int order;
		struct page *p;

		order = sprng() % 3;

		p = page_alloc(order, 0);
		if (p) {
			n += 1 << order;
			pages[i++] = ((struct pages) { p, order, });
		}

		dbg("pa(%2u): pfn=%lx tot=%lu\n", order, page_to_pfn(p), n);

		if (!p && order == 0)
			break;
	}

	dbg("no memory left! (nbr alloc = %u, total = %lu)\n", i, n);

	f = free_all(i);
	if (f != n) {
		printf("ERROR! %s():%s: f = %lu but n was %lu\n", __func__, name, f, n);
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

	for (i = 0; i < 64; i++) {
		RESULT(test_alloc_free_all, free_all_tail);
		RESULT(test_alloc_free_all, free_all_head);
	}

	printf("TEST page-alloc: fail = %u, succeed = %i\n", ko, ok);
	thread_sleep();
}

void kapi_start(void)
{
	printf(os_version);

	thread_create(&t, 2, fun, NULL);
	thread_start(&t);
}
