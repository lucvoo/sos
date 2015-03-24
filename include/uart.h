#ifndef	_UART_H_
#define	_UART_H_

struct uart;

struct uart_ops {
	int			(*start)(struct uart *);
	int			(*stop)(struct uart *);
};

struct uart {
	const char*		name;
	struct uart_ops*	ops;

	struct uart*		next;
};

#endif
