#ifndef	_SMP_INITCALL_H_
#define	_SMP_INITCALL_H_


struct smp_initcall {
	void (*fun)(uint cpu);
};

#define	__smp_initcall(level,fn) \
	static struct smp_initcall __smp_initcall_##fn __used __section(".smp_initcall" level) = { \
		.fun = fn, \
	}

#define	__smp_init	__section(".smp_init.text")

#define	pure_smp_initcall(fn)		__smp_initcall("0",fn)
#define	early_smp_initcall(fn)		__smp_initcall("1",fn)
#define	core_smp_initcall(fn)		__smp_initcall("3",fn)
#define	late_smp_initcall(fn)		__smp_initcall("8",fn)

#endif
