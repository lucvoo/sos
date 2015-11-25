#include <kapi/kapi.h>
#include <version.h>
#include <utils.h>
#include <timer.h>


static int timeouts[] = { 10, 1, 15, 20, 3, };
static struct timer timers[ARRAY_SIZE(timeouts)];

static void timer_action(void *data)
{
	unsigned long *exp = data;

	printf("\ntimeout: exp = %lu\n", *exp);
}

void kapi_start(void)
{
	unsigned int i;

	printf(os_version);

	for (i = 0; i < ARRAY_SIZE(timeouts); i++) {
		timers[i].exp = timeouts[i] * HZ;
		timers[i].action = timer_action;
		timers[i].data = &timers[i].exp;
		timer_add(&timers[i]);
	}
}
