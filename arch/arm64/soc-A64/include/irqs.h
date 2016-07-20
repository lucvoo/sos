#ifndef _SOC_IRQS_H_
#define _SOC_IRQS_H_


#define	SGI(N)		N
#define	PPI(N)		(16+N)
#define	SPI(N)		(32+N)

#define GIC_TIMER_TZ_IRQ	PPI(13)
#define GIC_TIMER_NS_IRQ	PPI(14)
#define GIC_TIMER_VIRT_IRQ	PPI(11)
#define GIC_TIMER_HYP_IRQ	PPI(10)

#endif
