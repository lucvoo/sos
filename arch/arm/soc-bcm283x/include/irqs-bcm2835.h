#ifndef	_BCM2835_IRQS_H_
#define	_BCM2835_IRQS_H_

#define	ARM_IRQ(N)	((N) + 64)
#define	GPU_IRQ(N)	((N) + 0)

#define	IRQ_TIMER	GPU_IRQ(1)

#endif
