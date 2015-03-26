#ifndef _UARTDEV_H_
#define _UARTDEV_H_

struct uart;

struct uart* uart_get_dev(const char *name);

int uart_start(struct uart *up);
int uart_stop(struct uart *up);

typedef void (*uart_rx_insert_t)(struct uart *, void *data, unsigned char ch, unsigned int flag);
typedef void (*uart_rx_handle_t)(struct uart *, void *data);
void uart_rx_handler(struct uart *up, void *data, uart_rx_insert_t, uart_rx_handle_t);

#endif
