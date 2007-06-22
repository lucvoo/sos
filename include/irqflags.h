#ifndef	_IRQFLAGS_H_
#define	_IRQFLAGS_H_

#include <arch/irqflags.h>

#define __local_irq_ena()	__raw_irq_ena()
#define __local_irq_dis()	__raw_irq_dis()
#define __local_irq_save()	__raw_irq_save()
#define __local_irq_rest(flags)	__raw_irq_rest(flags)

#endif
