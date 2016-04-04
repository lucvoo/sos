#include <sleep.h>
#include <sched.h>
#include <round.h>
#include <hz.h>


void sleep(ulong secs)
{
	ulong ticks = (secs * HZ);

	do {
		ticks = thread_schedule_timeout(ticks);
	} while (ticks);
}

void msleep(ulong msecs)
{
	ulong ticks = 1 + (msecs * HZ) / 1000;

	if (msecs >= 1000)
		return sleep(DIV_ROUND_UP(msecs, 1000));

	do {
		ticks = thread_schedule_timeout(ticks);
	} while (ticks);
}

void usleep(ulong usecs)
{
	ulong ticks = 1 + (usecs * HZ) / 1000000;

	if (usecs >= 1000)
		return msleep(DIV_ROUND_UP(usecs, 1000));

	do {
		ticks = thread_schedule_timeout(ticks);
	} while (ticks);
}
