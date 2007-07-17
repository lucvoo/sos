#include <diag.h>
#include <symbols.h>


void _os_diag_write_string(const char* str)
{
	unsigned int c;

	while ((c = *str++))
		_os_diag_write_char(c);
}

void _os_diag_write_buf(const char* str, unsigned int n)
{
	while (n--)
		_os_diag_write_char(*str++);
}
weak_alias(_os_diag_write_buf, putx);
