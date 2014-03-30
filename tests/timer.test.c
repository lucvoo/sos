#include <kapi/kapi.h>
#include <version.h>
#include <utils.h>
#include <timer.h>


static int timeouts[] = { 10, 1, 15, 20, 3, 0, };
static struct timer timers[ARRAY_SIZE(timeouts)];

void kapi_start(void)
{
	unsigned int i;

	printf(os_version);

	for (i = 0; i < ARRAY_SIZE(timeouts); i++) {
		timers[i].exp = timeouts[i];
		timer_add(&timers[i]);
	}
}
