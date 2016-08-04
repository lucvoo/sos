#include <printk.h>
#include <stdarg.h>
#include <lock.h>
#include <ansi.h>


extern struct lock printf_lock;

static const char *pref[] = {
	[LOG_LEVEL_EMERG] =	 ANSI_RED "EMERG" ANSI_DEF,
	[LOG_LEVEL_ALERT] =	 ANSI_RED "ALERT" ANSI_DEF,
	[LOG_LEVEL_CRIT] =	 ANSI_RED "CRIT"  ANSI_DEF,
	[LOG_LEVEL_ERROR] =	 ANSI_RED "ERROR" ANSI_DEF,
	[LOG_LEVEL_WARN] =	 ANSI_ORA "WARN"  ANSI_DEF,
	[LOG_LEVEL_NOTE] =	 ANSI_YEL "NOTE"  ANSI_DEF,
	[LOG_LEVEL_INFO] =	 ANSI_YEL "INFO"  ANSI_DEF,
	[LOG_LEVEL_DEBUG] =	 ANSI_YEL "DEBUG" ANSI_DEF,
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
