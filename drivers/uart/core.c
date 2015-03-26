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

void uart_rx_handler(struct uart *up, void *data, uart_rx_insert_t insert , uart_rx_handle_t handle)
{
	up->rx_data = data;
	up->rx_insert = insert;
	up->rx_handle = handle;
}
