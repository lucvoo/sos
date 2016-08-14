#include <diag.h>


void _os_diag_hexval(unsigned long val, uint sep)
{
	int n;

	for (n = sizeof(val)*2; n--;) {
		unsigned int d = (val >> (4*n)) & 0xf;

		if (d > 9)
			d += 'A' - 10;
		else
			d += '0';

		_os_diag_write_char(d);
	}

	if (sep)
		_os_diag_write_char(sep);
}
