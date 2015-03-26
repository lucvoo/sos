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

	void*			rx_data;
	void			(*rx_insert)(struct uart *, void *data, unsigned char ch, unsigned int flag);
	void			(*rx_handle)(struct uart *, void *data);

	struct uart*		next;
};


#define	uart_inc_stats(p, x)	do ; while (0)	// FIXME


#define	UART_TX_ORDER		9
#define	UART_RX_ORDER		9

#define	CHF_NORMAL	0
#define	CHF_BI		1
#define	CHF_PE		2
#define	CHF_FE		3
#define	CHF_OE		4

#endif
