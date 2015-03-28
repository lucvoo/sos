#ifndef	_UART_CORE_H_
#define	_UART_CORE_H_

#include <uart.h>


struct uart;

struct uart_ops {
	int			(*start)(struct uart *);
	int			(*stop)(struct uart *);
};

struct uart {
	const char*		name;
	struct uart_ops*	ops;

	void*			rx_data;
	void			(*rx_insert)(struct uart *, void *data, unsigned char ch, unsigned int flag);
	void			(*rx_handle)(struct uart *, void *data);

	struct uart*		next;
};


#define	uart_inc_stats(p, x)	do ; while (0)	// FIXME

#endif
