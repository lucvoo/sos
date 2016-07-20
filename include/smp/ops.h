#ifndef	_SMP_OPS_H_
#define	_SMP_OPS_H_

struct thread;

struct smp_ops {
	int (*init)(void);
	int (*boot_cpu)(struct thread *idle, uint cpu);
	int (*init_cpu)(uint cpu);
};

extern struct smp_ops smp_ops;

#endif
