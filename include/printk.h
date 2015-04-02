#ifndef	_PRINTK_H_
#define	_PRINTK_H_

#define	pr_crit(fmt, ...)	printf("CRIT @ %s:%d:\n\t" fmt, __func__, __LINE__, ##__VA_ARGS__)
#define	pr_err(fmt, ...)	printf("ERR  @ %s:%d:\n\t" fmt, __func__, __LINE__, ##__VA_ARGS__)
#define	pr_warn(fmt, ...)	printf("WARN @ %s:%d:\n\t" fmt, __func__, __LINE__, ##__VA_ARGS__)
#define	pr_info(fmt, ...)	printf("INFO @ %s:%d:\n\t" fmt, __func__, __LINE__, ##__VA_ARGS__)
#define	pr_dbg(fmt, ...)	printf("DBG  @ %s:%d:\n\t" fmt, __func__, __LINE__, ##__VA_ARGS__)

#endif
