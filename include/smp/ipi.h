#ifndef _SMP_IPI_H_
#define _SMP_IPI_H_


#define	SMP_IPI_ALL	-1
#define	SMP_IPI_OTHER	-2
#define	SMP_IPI_SELF	-3

#define	SMP_IPI_PING		0
#define	SMP_IPI_PONG		1
#define	SMP_IPI_SCHEDULE	2

void __smp_ipi_send(uint cpu, uint msg);
void __smp_ipi_process(uint ipi);

#endif
