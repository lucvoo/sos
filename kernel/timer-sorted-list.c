#include <timer.h>


static struct timers {
	struct dlist_head	head;
} timers = {
	.head = DLIST_HEAD_INIT(timers.head),
};

#if defined(DEBUG)
static void timer_dump_timers(const char *fmt, ...)
{
	struct timer *cur;
	va_list ap;

	printf("TIMERS:");
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	printf(":\n");
	dlist_foreach_entry(cur, &timers.head, node) {
		printf("\t%p : %08lu\n", cur, cur->exp);
	}
	printf("\n");
}
#else
static void timer_dump_timers(const char *fmt, ...)
{
}
#endif

static void timer_reprogram(void)
{
}

void timer_add(struct timer *t)
{
	struct timer *cur;

	dlist_foreach_entry(cur, &timers.head, node) {
		if (cur->exp > t->exp)
			break;
	}

	dlist_insert(cur->node.prev, &t->node, &cur->node);

	timer_dump_timers("timer_add(%p:%lu)", t, t->exp);
	timer_reprogram();
}

void timer_del(struct timer *t)
{
	dlist_del(&t->node);
}
