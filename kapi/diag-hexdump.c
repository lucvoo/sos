#include <diag.h>


static inline unsigned int mapchar(unsigned int c)
{
	if (c >= ' ' && c <= '~')
		return c;
	else
		return '.';
}

static void _os_diag_write_hexbyte(unsigned int val)
{
	unsigned int n;

	for (n = 2; n--;) {
		unsigned int d = (val >> (4*n)) & 0xf;

		if (d > 9)
			d += 'A' - 10;
		else
			d += '0';

		_os_diag_write_char(d);
	}
}

static void _os_diag_hexdump_line(const unsigned char *buff, unsigned int len, int ascii)
{
	unsigned int i;

	_os_diag_printf("%p  ", buff);

	if (1) {
		// Hex part
		for (i = 0; i < len; i++) {
			_os_diag_write_hexbyte(buff[i]);
			_os_diag_write_char(' ');
		}
		for (; i < 16; i++)
			_os_diag_write_buf("   ", 3);
	}

	if (ascii) {
		// Ascii part
		for (i = 0; i < len; i++)
			_os_diag_write_char(mapchar(buff[i]));
		for (; i < 16; i++)
			_os_diag_write_char(' ');
	}

	_os_diag_write_char('\n');
}

void _os_diag_hexdump(const char *msg, const void *buff, unsigned int len, int ascii)
{
	if (msg) {
		_os_diag_write_string(msg);
		_os_diag_write_char('\n');
	}

	// Dump the buffer by lines of maximum 16 bytes
	while (len) {
		unsigned int n = len > 16 ? 16 : len;

		_os_diag_hexdump_line(buff, n, ascii);
		buff += n;
		len -= n;
	}
}

static void _os_diag_hexdump32_line(const unsigned long *buff, unsigned int len)
{
	unsigned int i;

	_os_diag_printf("%p  ", buff);

	if (1) {
		// Hex part
		for (i = 0; i < len; i++) {
			_os_diag_hexval(buff[i], ' ');
		}
		for (; i < 4; i++)
			_os_diag_write_buf("         ", 9);
	}

	_os_diag_write_char('\n');
}

void _os_diag_hexdump32(const char *msg, const void *buff, unsigned int len)
{
	if (msg) {
		_os_diag_write_string(msg);
		_os_diag_write_char('\n');
	}

	// Dump the buffer by lines of maximum 4 words
	while (len) {
		unsigned int n = len > 4 ? 4 : len;

		_os_diag_hexdump32_line(buff, n);
		buff += n * 4;
		len -= n;
	}
}
