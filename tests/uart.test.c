#include <kapi/kapi.h>
#include <version.h>
#include <uartdev.h>
#include <uart.h>
#include <sched.h>


static struct thread a __uninit;

struct rx_buf {
	unsigned int	n;
	unsigned char	buf[16];
};

static void rx_insert(struct uart *up, void *data, unsigned char ch, unsigned int flag)
{
	// This run in ISR context
	struct rx_buf *rbuf = data;

	//printf("%s(%02x, %x)\n", __func__, ch, flag);

	if (flag == 0)
		rbuf->buf[rbuf->n++] = ch;
}

static void rx_handle(struct uart *up, void *data)
{
	// This run in DSR context
	struct rx_buf *rbuf = data;
	unsigned int i;

	printf("%s():\n", __func__);

	// racy but it's just an example
	for (i = 0; i < rbuf->n; i++) {
		printf("\t%02x\n", rbuf->buf[i]);
	}
	rbuf->n = 0;
}

static void fun(void* data)
{
	static struct rx_buf rbuf;
	struct uart *up;
	int n = 0;

	up = uart_get_dev("uart0");
	uart_rx_handler(up, &rbuf, rx_insert, rx_handle);
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
