#include <hexdump.h>

static inline unsigned int mapchar(unsigned int c)
{
	if (c >= ' ' && c <= '~')
		return c;
	else
		return '.';
}

static void hexdump_line(const unsigned char *buff, unsigned int len, int ascii)
{
	unsigned int i;

	printf("%p  ", buff);

	if (1) {
		// Hex part
		for (i = 0; i < len; i++)
			printf("%02x ", buff[i]);
	}

	if (ascii) {
		// Ascii part
		for (; i < 16; i++)	// pad to full line
			printf("   ");
		for (i = 0; i < len; i++)
			printf("%c", mapchar(buff[i]));
		for (; i < 16; i++)
			printf(" ");
	}

	printf("\n");
}

void hexdump(const void *buff, unsigned int len)
{
	// Dump the buffer by lines of maximum 16 bytes
	while (len) {
		unsigned int n = len > 16 ? 16 : len;

		hexdump_line(buff, n, 0);
		buff += n;
		len -= n;
	}
}

#if 0
void hexdumpf(const void *addr, unsigned int len, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);

	hexdump(addr, len);
}
#endif

void binhex(const void *addr, unsigned int len)
{
	const unsigned char *src = addr;

	while (len--)
		printf("%02X", *src++);
	printf("\n");
}
