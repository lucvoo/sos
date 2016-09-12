#include <timer.h>
#include <timerdev.h>
#include <lock.h>
#include <smp.h>
#include <init.h>


static struct timerdev *td;

static struct timers {
	struct lock		lock;
	struct dlist_head	head;
} timers[NR_CPUS];


static void timers_init(void)
{
	foreach_cpu(cpu)
		dlist_init(&timers[cpu].head);
}
pure_initcall(timers_init);

static void timer_dbg(struct timers *ts, struct timer *t, int dump, const char *fmt, ...)
{
#if defined(DEBUG)
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
		dlist_foreach_entry(cur, &ts->head, node) {
			printf("\t%p : %08lu\n", cur, cur->exp);
		}
	} else
		printf("\n");
#endif
}

static struct timers *get_timers(void)
{
	// FIXME: this, like all per-cpu refs, need some protection
	//	  against switching to another CPU

	return &timers[__cpuid()];
}


static void timer_program(struct timers *ts)
{
	struct timer *t;
	int rc;

loop:
	if (dlist_is_empty(&ts->head)) {
		timer_dbg(ts, NULL, 0, "---");
		return;
	}

	t = dlist_peek_entry(&ts->head, struct timer, node);
	if (td->next_abs)
		rc = td->next_abs(td, t->exp);
	else {
		unsigned long now;

		// FIXME: need to decouple the timer clock from the 'now' clock
		now = td->now(td);
		rc = td->next_rel(td, t->exp - now);
	}

	timer_dbg(ts, t, 0, "prg");
	if (rc) {
		dlist_del(&t->node);
		timer_dbg(ts, NULL, 0, "!!!");
		t->action(t->data);
		goto loop;
	}
}

static void timer_add_locked(struct timers *ts, struct timer *t)
{
	struct timer *cur;
	unsigned long exp = t->exp;

	dlist_foreach_entry(cur, &ts->head, node) {
		if (cur->exp > exp)
			break;
	}

	dlist_insert(cur->node.prev, &t->node, &cur->node);

	timer_dbg(ts, t, 1, "add", t, t->exp);

	// FIXME: we need to guarantee that this will done on ts' CPU
	timer_program(ts);
}

void timer_add_abs(struct timer *t)
{
	struct timers *ts = get_timers();

	lock_acq_irq(&ts->lock);

	timer_add_locked(ts, t);

	lock_rel_irq(&ts->lock);
}

void timer_add_rel(struct timer *t, ulong ticks)
{
	struct timers *ts = get_timers();

	lock_acq_irq(&ts->lock);

	t->exp = td->now(td) + ticks;
	timer_add_locked(ts, t);

	lock_rel_irq(&ts->lock);
}

void timer_del(struct timer *t)
{
	struct timers *ts = get_timers();

	lock_acq_irq(&ts->lock);

	dlist_del(&t->node);
	timer_program(ts);

	lock_rel_irq(&ts->lock);
}


static void timeout_process(struct timerdev *td)
{
	struct timers *ts = get_timers();
	struct timer *cur;
	struct timer *next;

	lock_acq(&ts->lock);

	dlist_foreach_entry_safe(cur, next, &ts->head, node) {
		long delta = cur->exp - td->now(td);

		if (delta > 0)
			break;
		dlist_del(&cur->node);
		timer_dbg(ts, cur, 0, "exp");
		cur->action(cur->data);
	}

	timer_program(ts);

	lock_rel(&ts->lock);
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

