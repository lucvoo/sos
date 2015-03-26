#include <kapi/kapi.h>
#include <version.h>
#include <uartdev.h>
#include <uart.h>
#include <sched.h>


static struct thread a __uninit;

static void fun(void* data)
{
	struct uart *up;
	int n = 0;

	up = uart_get_dev("uart0");
	printf("up = %p\n", up);
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
