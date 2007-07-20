#include <diag.h>
#include <symbols.h>
#include <irqflags.h>


void _os_diag_write_string(const char* str)
{
	unsigned int c;
	unsigned long flags;

	flags = __local_irq_save();
	while ((c = *str++))
		_os_diag_write_char(c);
	__local_irq_rest(flags);
}

void _os_diag_write_buf(const char* str, unsigned int n)
{
	unsigned long flags;

	flags = __local_irq_save();
	while (n--)
		_os_diag_write_char(*str++);
	__local_irq_rest(flags);
}
weak_alias(_os_diag_write_buf, putx);
