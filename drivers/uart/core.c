#include <uartdev.h>
#include <uart.h>


int uart_start(struct uart *up)
{
	return up->ops->start(up);
}

int uart_stop(struct uart *up)
{
	return up->ops->stop(up);
}
