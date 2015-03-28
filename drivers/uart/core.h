#ifndef	_UART_CORE_H_
#define	_UART_CORE_H_

#include <uart.h>
#include <uartdev.h>


struct uart;

struct uart_ops {
	int			(*start)(struct uart *);
	int			(*stop)(struct uart *);
};

struct uart {
	const char*		name;
	struct uart_ops*	ops;

	struct uart_rx*		rx;

	struct uart*		next;
};


#define	uart_inc_stats(p, x)	do ; while (0)	// FIXME

#endif
