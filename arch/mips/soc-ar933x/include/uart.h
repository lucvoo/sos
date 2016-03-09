#ifndef	_AR933X_UART_H_
#define	_AR933X_UART_H_

#define	UART_DATA	0x00
#define	UART_DATA_TX		(1 << 9)
#define	UART_DATA_RX		(1 << 8)
#define	UART_DATA_MSK		(0xFF)

#define	UART_CS		0x04
#define	UART_CLOCK	0x08
#define	UART_INT	0x0C
#define	UART_INT_EN	0x10

#endif
