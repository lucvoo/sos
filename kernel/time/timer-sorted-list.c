#include <timer.h>
#include <timerdev.h>
#include <lock.h>


static struct timerdev *td;

static struct timers {
	struct lock		lock;
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

static void timer_program(void)
{
	struct timer *t;
	int rc;

loop:
	if (dlist_is_empty(&timers.head)) {
		timer_dbg(NULL, 0, "---");
		return;
	}

	t = dlist_peek_entry(&timers.head, struct timer, node);
	if (td->next_abs)
		rc = td->next_abs(td, t->exp);
	else {
		unsigned long now;

		// FIXME: need to decouple the timer clock from the 'now' clock
		now = td->now(td);
		rc = td->next_rel(td, t->exp - now);
	}

	timer_dbg(t, 0, "prg");
	if (rc) {
		dlist_del(&t->node);
		timer_dbg(NULL, 0, "!!!");
		t->action(t->data);
		goto loop;
	}
}

static void timer_add_locked(struct timer *t)
{
	struct timer *cur;
	unsigned long exp = t->exp;

	dlist_foreach_entry(cur, &timers.head, node) {
		if (cur->exp > exp)
			break;
	}

	dlist_insert(cur->node.prev, &t->node, &cur->node);

	timer_dbg(t, 1, "add", t, t->exp);
	timer_program();
}

void timer_add(struct timer *t)
{
	lock_acq_irq(&timers.lock);

	timer_add_locked(t);

	lock_rel_irq(&timers.lock);
}

void timer_add_rel(struct timer *t)
{
	unsigned long now;

	lock_acq_irq(&timers.lock);

	now = td->now(td);
	t->exp += now;
	timer_add_locked(t);

	lock_rel_irq(&timers.lock);
}

void timer_del(struct timer *t)
{
	lock_acq_irq(&timers.lock);

	dlist_del(&t->node);
	timer_program();

	lock_rel_irq(&timers.lock);
}


static void timeout_process(struct timerdev *td)
{
	struct timer *cur;
	struct timer *next;

	lock_acq(&timers.lock);

	dlist_foreach_entry_safe(cur, next, &timers.head, node) {
		long delta = cur->exp - td->now(td);

		if (delta > 0)
			break;
		dlist_del(&cur->node);
		timer_dbg(cur, 0, "exp");
		cur->action(cur->data);
	}

	timer_program();

	lock_rel(&timers.lock);
}

unsigned long timerdev_read(void)
{
	return td->now(td);
}

int timerdev_register(struct timerdev *newtd)
{
	newtd->handler = timeout_process;
	td = newtd;

	return 0;
}

