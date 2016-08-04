#ifndef	_PRINTK_H_
#define	_PRINTK_H_

#define	LOG_LEVEL_EMERG	0
#define	LOG_LEVEL_ALERT	1
#define	LOG_LEVEL_CRIT	2
#define	LOG_LEVEL_ERROR	3
#define	LOG_LEVEL_WARN	4
#define	LOG_LEVEL_NOTE	5
#define	LOG_LEVEL_INFO	6
#define	LOG_LEVEL_DEBUG	7

#define	pr_emerg(fmt, ...)	log_(LOG_LEVEL_EMERG, fmt, ##__VA_ARGS__)
#define	pr_alert(fmt, ...)	log_(LOG_LEVEL_ALERT, fmt, ##__VA_ARGS__)
#define	pr_crit(fmt, ...)	log_(LOG_LEVEL_CRIT,  fmt, ##__VA_ARGS__)
#define	pr_err(fmt, ...)	log_(LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define	pr_warn(fmt, ...)	log_(LOG_LEVEL_WARN,  fmt, ##__VA_ARGS__)
#define	pr_note(fmt, ...)	log_(LOG_LEVEL_NOTE,  fmt, ##__VA_ARGS__)
#define	pr_info(fmt, ...)	log_(LOG_LEVEL_INFO,  fmt, ##__VA_ARGS__)
#define	pr_dbg(fmt, ...)	log_(LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)


#ifndef	CONFIG_LOG_LEVEL
#define	CONFIG_LOG_LEVEL	LOG_LEVEL_NOTE
#endif
#ifndef	LOG_LEVEL
#define	LOG_LEVEL		CONFIG_LOG_LEVEL
#endif


#define	log_(LVL, fmt, ...)		\
	do {				\
		if (LVL <= LOG_LEVEL)	\
			printf_log(LVL, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__);	\
	} while (0)

void printf_log(uint level, const char *file, const char *func, int line, const char *fmt, ...);

#endif
