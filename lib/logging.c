#include <printk.h>
#include <stdarg.h>
#include <lock.h>


extern struct lock printf_lock;

static const char *pref[] = {
	[LOG_LEVEL_EMERG] =	 "EMERG",
	[LOG_LEVEL_ALERT] =	 "ALERT",
	[LOG_LEVEL_CRIT] =	 "CRIT",
	[LOG_LEVEL_ERROR] =	 "ERROR",
	[LOG_LEVEL_WARN] =	 "WARN",
	[LOG_LEVEL_NOTE] =	 "NOTE",
	[LOG_LEVEL_INFO] =	 "INFO",
	[LOG_LEVEL_DEBUG] =	 "DEBUG",
};

static void vprintf_log(uint level, const char *file, const char *func, int line, const char *fmt, va_list ap)
{
	if (level > LOG_LEVEL_DEBUG)
		level = LOG_LEVEL_DEBUG;

	__printf("%s: ", pref[level]);
	__vprintf(fmt, ap);
	__printf(" @ %s:%s:%d\n", file, func, line);
}

void printf_log(uint level, const char *file, const char *func, int line, const char *fmt, ...)
{
	ulong flags;
	va_list ap;

	va_start(ap, fmt);
	flags = lock_acq_save(&printf_lock);
	vprintf_log(level, file, func, line, fmt, ap);
	lock_rel_rest(&printf_lock, flags);
	va_end(ap);
}
