#ifndef _UARTDEV_H_
#define _UARTDEV_H_

struct uart;

struct uart* uart_get_dev(const char *name);

#endif
