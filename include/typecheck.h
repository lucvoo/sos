#ifndef	_TYPECHECK_H_
#define	_TYPECHECK_H_

#define	typecheck(type, exp)		\
	({	type __t;		\
		typeof(exp) __e;	\
		(void)(&__t == &__e);	\
	})

#endif
