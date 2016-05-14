#ifndef _SMP_IPI_H_
#define _SMP_IPI_H_


#define	SMP_IPI_ALL	-1
#define	SMP_IPI_OTHER	-2
#define	SMP_IPI_SELF	-3

#define	SMP_IPI_PING		0x00000001
#define	SMP_IPI_PONG		0x00000002
#define	SMP_IPI_SCHEDULE	0x00000004

void __smp_ipi_send(unsigned int cpu, unsigned int msg);
void __smp_ipi_process(unsigned long msg);

#endif
