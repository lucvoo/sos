#include <kapi/kapi.h>
#include <version.h>
#include <uartdev.h>
#include <uart.h>
#include <circbuf.h>
#include <semaphore.h>
#include <sched.h>


#define	UART_RXBUF_ORDER	8

struct uart_char {
	unsigned char	ch;
	unsigned char	fl;
};

struct uart_rxbuf {
	struct uart_rx		rx;

	//struct uart*		up;
	struct semaphore	sem;

	struct circbuf		cb;
	struct uart_char	buf[1 << UART_RXBUF_ORDER];
};

/*
 * Push each received char into a circular buffer
 * Wakeup the waiting thread if the buffer (will) overflow,
 * a newline is received or some RX errors occured.
*/
static void rxbuf_push_char(struct uart_rx* rx, unsigned int ch, unsigned int fl)
{
	struct uart_rxbuf *rxbuf = container_of(rx, struct uart_rxbuf, rx);
	struct uart_char chfl = { .ch = ch, .fl = fl, };
	unsigned int tail = rxbuf->cb.tail;

	// This run in ISR context
	if (!circbuf_free(&rxbuf->cb, UART_RXBUF_ORDER)) {
		semaphore_post(&rxbuf->sem);
		return;
	}

	tail = rxbuf->cb.tail;
	rxbuf->buf[tail] = chfl;
	rxbuf->cb.tail = (tail + 1) % (1 << UART_RXBUF_ORDER);

	if (ch == '\r')
		ch = '\n';
	if (ch == '\n' || fl != CHF_NORMAL || !circbuf_free(&rxbuf->cb, UART_RXBUF_ORDER))
		semaphore_post(&rxbuf->sem);
}


static void uart_rxbuf_attach(struct uart *up, struct uart_rxbuf *rxbuf)
{
	rxbuf->rx.push_char = rxbuf_push_char;
	//rxbuf->up = up;

	semaphore_init(&rxbuf->sem, 0);
	uart_rx_handler(up, &rxbuf->rx);
}


static void test_rxbuf(void* data)
{
	struct uart_rxbuf *rxbuf = data;
	struct uart *up;

	up = uart_get_dev("uart0");
	uart_rxbuf_attach(up, rxbuf);
	uart_start(up);

	while (1) {
		unsigned int head;

		semaphore_wait(&rxbuf->sem);

		printf("chars in buffer:\n\t");
		circbuf_iter_head(head, &rxbuf->cb, UART_RXBUF_ORDER) {
			unsigned int ch = rxbuf->buf[head].ch;

			printf(" %02x", ch);
			if (ch == '\n')
				printf("\n\t");
		}
		printf("\n");
	}
}


static struct thread thr __uninit;

static struct uart_rxbuf rxbuf = {
};

void kapi_start(void)
{
	printf(os_version);

	thread_create(&thr, 2, test_rxbuf, &rxbuf, NULL, 0);
	thread_start(&thr);
}
