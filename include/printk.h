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

#define	pr_emerg(fmt, ...)	log_(LOG_LEVEL_EMERG, "EMERG", fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define	pr_alert(fmt, ...)	log_(LOG_LEVEL_ALERT, "ALERT", fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define	pr_crit(fmt, ...)	log_(LOG_LEVEL_CRIT,  "CRIT ", fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define	pr_err(fmt, ...)	log_(LOG_LEVEL_ERROR, "ERROR", fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define	pr_warn(fmt, ...)	log_(LOG_LEVEL_WARN,  "WARN ", fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define	pr_note(fmt, ...)	log_(LOG_LEVEL_NOTE,  "NOTE ", fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define	pr_info(fmt, ...)	log_(LOG_LEVEL_INFO,  "INFO ", fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define	pr_dbg(fmt, ...)	log_(LOG_LEVEL_DEBUG, "DEBUG", fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__)


#ifndef	CONFIG_LOG_LEVEL
#define	CONFIG_LOG_LEVEL	LOG_LEVEL_NOTE
#endif
#ifndef	LOG_LEVEL
#define	LOG_LEVEL		CONFIG_LOG_LEVEL
#endif


#define	log_(LVL, M, fmt, F, f, L, ...)						\
	do {									\
		if (LVL <= LOG_LEVEL)						\
			printf("%s @ %s:%d:\n\t" fmt, M, f, L, ##__VA_ARGS__);	\
	} while (0)

#endif
