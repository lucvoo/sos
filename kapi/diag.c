#include <diag.h>


void _os_diag_write_string(const char* str)
{
	unsigned int c;

	while ((c = *str++))
		_os_diag_write_char(c);
}
