#include <kapi/kapi.h>
#include <version.h>
#include <uartdev.h>
#include <uart.h>
#include <sched.h>


static struct thread a __uninit;

struct rx_handler {
	struct uart_rx	rx;

	// buffer
	unsigned int	n;
	unsigned char	buf[16];
};

static void rx_push_char(struct uart_rx *urx, unsigned int ch, unsigned int flag)
{
	// This run in ISR context
	struct rx_handler *rx = container_of(urx, struct rx_handler, rx);

	//printf("%s(%02x, %x)\n", __func__, ch, flag);

	if (flag == 0)
		rx->buf[rx->n++] = ch;
}

static void rx_handle(struct uart_rx *urx)
{
	// This run in DSR context
	struct rx_handler *rx = container_of(urx, struct rx_handler, rx);
	unsigned int i;

	printf("%s():\n", __func__);

	// racy but it's just an example
	for (i = 0; i < rx->n; i++) {
		printf("\t%02x\n", rx->buf[i]);
	}
	rx->n = 0;
}

static struct rx_handler rx_handler = {
	.rx = {
		.push_char	= rx_push_char,
		.handle		= rx_handle,
	},
};

static void fun(void* data)
{
	struct uart *up;
	int n = 0;

	up = uart_get_dev("uart0");
	uart_rx_handler(up, &rx_handler.rx);
	uart_start(up);

	while (1) {
		thread_schedule_timeout(4096);
		printf("... (%d)\n", n++);
	}
}


void kapi_start(void)
{
	printf(os_version);

	thread_create(&a, 2, fun, 0, NULL, 0);
	thread_start(&a);
}
