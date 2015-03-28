#include <uartdev.h>
#include "core.h"


int uart_start(struct uart *up)
{
	return up->ops->start(up);
}

int uart_stop(struct uart *up)
{
	return up->ops->stop(up);
}

void uart_rx_handler(struct uart *up, struct uart_rx *rx)
{
	up->rx = rx;
}
