#ifndef _UARTDEV_H_
#define _UARTDEV_H_

#include <uart.h>

struct uart;

struct uart* uart_get_dev(const char *name);

int uart_start(struct uart *up);
int uart_stop(struct uart *up);


struct uart_rx {
	void		(*push_char)(struct uart_rx*, unsigned int ch, unsigned int flag);
	void		(*handle)(struct uart_rx*);
};

void uart_rx_handler(struct uart *up, struct uart_rx *rx);

#endif
