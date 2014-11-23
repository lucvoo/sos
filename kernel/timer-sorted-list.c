#include <timer.h>
#include <timerdev.h>


static struct timerdev *td;

static struct timers {
	struct dlist_head	head;
} timers = {
	.head = DLIST_HEAD_INIT(timers.head),
};

#if defined(DEBUG)
static void timer_dbg(struct timer *t, int dump, const char *fmt, ...)
{
	struct timer *cur;
	va_list ap;

	printf("\nTIMERS @%08lu:", td->now(td));
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	if (t)
		printf(": %p->exp @%08lu", t, t->exp);
	if (dump) {
		printf(":\n");
		dlist_foreach_entry(cur, &timers.head, node) {
			printf("\t%p : %08lu\n", cur, cur->exp);
		}
	} else
		printf("\n");
}
#else
static void timer_dbg(struct timer *t, int dump, const char *fmt, ...)
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

	timer_dbg(t, 1, "add", t, t->exp);
	timer_reprogram();
}

void timer_add_rel(struct timer *t)
{
	unsigned long now;

	now = td->now(td);
	t->exp += now;
	timer_add(t);
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
		timer_dbg(cur, 0, "exp");
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

