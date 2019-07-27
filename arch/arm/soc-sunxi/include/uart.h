#ifndef _SOC_UART_H_
#define _SOC_UART_H_

#include <hw/16550.h>

#define	UART_USR	31	// UART Status Register
#define	UART_TFL	32	// Tx FIFO Level
#define	UART_RFL	33	// Rx FIFO Level
#define	UART_HSK	34	// DMA HandShake config Register
#define	UART_HLT	41	// Halt Tx register


#define	UARTX_SIZE	0x400
#define	UARTX_BASE(P)	(UART_BASE + (P) * 0x400)


#endif
