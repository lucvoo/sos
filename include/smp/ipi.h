#ifndef _SMP_IPI_H_
#define _SMP_IPI_H_


#define	SMP_IPI_PING		0x00000001
#define	SMP_IPI_PONG		0x00000002
#define	SMP_IPI_SCHEDULE	0x00000004

void __smp_ipi_send(unsigned int cpu, unsigned int msg);

#endif
