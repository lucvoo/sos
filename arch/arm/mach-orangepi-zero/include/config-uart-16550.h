#ifndef	_MACH_CFG_UART_16550_H_
#define	_MACH_CFG_UART_16550_H_

#include <soc/16550.h>

#define	UART_16550_NBR_PORTS	4

#endif


#ifdef	UART_ENTRY
	UART_ENTRY(0, UARTX_BASE(0), UARTX_IRQ(0))
#endif
