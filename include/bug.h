#ifndef _BUG_H_
#define _BUG_H_


#ifdef CONFIG_BUG
#include <arch/bug.h>

#define	BUG()								\
	do {								\
		__BUG(__FILE__, __func__, __LINE__);			\
		__builtin_unreachable();				\
	} while (0)

#define	BUG_ON(cond)							\
	do {								\
		if (__builtin_constant_p(cond)) {			\
			if (cond)					\
				BUG();					\
		} else {						\
			__BUG_ON(__FILE__, __func__, __LINE__, cond);	\
		}							\
	} while (0)

#else	// CONFIG_BUG

#define BUG()

#define BUG_ON(condition) do { if (condition) ; } while(0)

#endif	// CONFIG_BUG
#endif
