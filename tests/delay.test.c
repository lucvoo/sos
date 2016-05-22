#include <kapi/kapi.h>
#include <version.h>
#include <utils.h>
#include <timer.h>
#include <delay.h>
#include <timerdev.h>


static struct timer timer;
static volatile int next;

static void timer_action(void *data)
{
	next = 1;
}

void kapi_start(void)
{
	ulong t0;

	printf(os_version);

	timer.exp = 10 * HZ;
	timer.action = timer_action;
	timer.data = &t0;

	while (1) {
		int n;

		next = 0;
		t0 = timerdev_read();

#define	SECS	10
		timer_add_rel(&timer, SECS * HZ);

		n = 0;
#define	STEPS 1000
		do {
			udelay(STEPS);
			n++;
		} while (next == 0);
		printf("n = %d (must be ~ %d)\n", n * STEPS, SECS * 1000000);
	}
}
