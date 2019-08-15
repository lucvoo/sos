#include <kapi/kapi.h>
#include <version.h>
#include <sched.h>
#include <diag.h>
#include <delay.h>


void kapi_start(void)
{
	while (1) {
		_os_diag_write_char('\n');
		for (int c = 0x20; c < 0x7f; c++)
			_os_diag_write_char(c);
		mdelay(1);
	}
}
