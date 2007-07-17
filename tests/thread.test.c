#include <version.h>
#include <diag.h>
#include <thread.h>

static struct thread a, b;

static void fun(void* data)
{
	const char* str = data;

	do {
		_os_diag_write_string(str);
		thread_yield();
	} while (1);
}

void kapi_start(void)
{
	_os_diag_write_string(os_version);

	thread_create(&a, 2, fun, "thread a\n");
	thread_create(&b, 2, fun, "thread b\n");
}
