#ifndef	_ARCH_MSR_H_
#define	_ARCH_MSR_H_


#define	msr_getval(REG)					\
	({	ulong __val;				\
		asm volatile(				\
		"mrs	%0, " #REG			\
		: "=r" (__val)				\
		);					\
		__val;					\
	})

#define	msr_setval(REG, VAL)				\
	({	ulong __val = VAL;			\
		asm volatile(				\
		"msr	" #REG ", %0"			\
		:					\
		: "r" (__val)				\
		);					\
	})

#define	msr_setbits(REG, SET)				\
	({	ulong __old, __new;			\
		__old = msr_getval(REG);		\
		__new = __old | (SET);			\
		msr_setval(REG, __new);			\
		__old;					\
	})

#define	msr_clrbits(REG, CLR)				\
	({	ulong __old, __new;			\
		__old = msr_getval(REG);		\
		__new =  __old & ~(CLR);		\
		msr_setval(REG, __new);			\
		__old;					\
	})

#define	msr_xorbits(REG, VAL)				\
	({	ulong __old, __new;			\
		__old = msr_getval(REG);		\
		__new = __old ^ (VAL);			\
		msr_setval(REG, __new);			\
		__old;					\
	})

#define	msr_clrset(REG, CLR, SET)			\
	({	ulong __old, __new;			\
		__old = msr_getval(REG);		\
		__new = (__old & ~(CLR)) | (SET);	\
		msr_setval(REG, __new);			\
		__old;					\
	})

#endif
