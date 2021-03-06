#ifndef _SMP_IPI_H_
#define _SMP_IPI_H_


#define	SMP_IPI_ALL	-1
#define	SMP_IPI_OTHER	-2
#define	SMP_IPI_SELF	-3

#define	SMP_IPI_RESCHED	0

void __smp_ipi_send(uint cpu, uint ipi);
void __smp_ipi_process(uint ipi);

#endif
