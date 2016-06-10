#ifndef	_SOC_16550_H_
#define	_SOC_16550_H_


#define	UARTX_IRQ(P)	(32 + (P))
#define	UARTX_BASE(P)	(0x01C28000 + (P) * 0x400)
#define	UART_IOSIZE	0x400

#define	UART_STEP	4
#define	UART_BITS	32


#include <hw/16550.h>

// non-standard registers
#define	UART_USR	31	// UART Status Register
#define	UART_TFL	32	// Tx FIFO Level
#define	UART_RFL	33	// Rx FIFO Level
#define	UART_HSK	34	// DMA HandShake config Register
#define	UART_HLT	41	// Halt Tx register

#endif
