#include <timer.h>
#include <timerdev.h>


static struct timerdev *td;

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
	struct timer *t;

	if (dlist_is_empty(&timers.head))
		return;

	t = dlist_peek_entry(&timers.head, struct timer, node);
	td->program(td, t->exp);
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
	timer_reprogram();
}


static void timeout_process(struct timerdev *td)
{
	struct timer *cur;
	struct timer *next;

	dlist_foreach_entry_safe(cur, next, &timers.head, node) {
		long delta = cur->exp - td->now(td);

		if (delta > 0)
			break;
		dlist_del(&cur->node);
		cur->action(cur->data);
	}

	timer_reprogram();
}

int timerdev_register(struct timerdev *newtd)
{
	newtd->handler = timeout_process;
	td = newtd;

	return 0;
}

