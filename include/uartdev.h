#ifndef _UARTDEV_H_
#define _UARTDEV_H_

struct uart;

struct uart* uart_get_dev(const char *name);

int uart_start(struct uart *up);
int uart_stop(struct uart *up);

#endif
