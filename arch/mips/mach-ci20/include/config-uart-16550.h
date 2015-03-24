#ifndef	_MACH_CFG_UART_16550_H_
#define	_MACH_CFG_UART_16550_H_

#include <soc/baseaddr.h>

#define	UART_16550_NBR_PORTS	5
#define	UART_IOSIZE		0x1000
#define	UART_IRQ_CTRL		"intc"
#define	UART_REG(N)		(N*4)

#endif


#ifdef	UART_ENTRY
	UART_ENTRY(0, UART0_BASE, 32+19)
	UART_ENTRY(4, UART4_BASE, 32+2)
#endif
