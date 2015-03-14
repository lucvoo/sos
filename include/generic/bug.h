#ifndef _GENERIC_BUG_H_
#define _GENERIC_BUG_H_


#define __BUG(file, func, line)						\
	do {								\
		printk("BUG @ %s:%s():%d!\n", file, func, line);	\
		panic("BUG!");						\
	} while (0)

#define __BUG_ON(file, func, line, cond)				\
	do {								\
		if (unlikely((cond) != 0))				\
			__BUG(file, func, line);			\
	} while(0)

#endif
