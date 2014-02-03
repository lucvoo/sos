#include "xprintf.h"
#include <printf.h>
#include <stdarg.h>
#include <string.h>

static void utostr(char *buf, unsigned size, unsigned val, unsigned base)
{
	unsigned digit;
	int i;

	buf[size] = '\0';
	for (i = size; --i >= 0;) {
		digit = val % base;
		val /= base;
		if (digit > 9)
			digit += 'A' - 10;
		else
			digit += '0';

		buf[i] = digit;
	}
}

unsigned xprintf(put_fn_t put, char *dest, unsigned size, const char *fmt, va_list ap)
{
	unsigned int c;
	unsigned int n = 0;
	unsigned prec;
	char *begin = dest;
	int idx;

	idx = 0;
	while ((c = (unsigned int)*fmt++)) {
		if (c != '%') {
			idx++;
			continue;
		}
		if (idx) {
			n = put(fmt - idx - 1, idx, dest, size);
			dest += n;
			size -= n;
			idx = 0;
		}

		prec = 0;
parse:
		switch ((c = *fmt++)) {
			unsigned int n;
			const char *s;
			char buf[33];
			unsigned int base;

		case 'c':
			n = va_arg(ap, unsigned int);

			buf[0] = n;
			buf[1] = 0;
			n = put(buf, 1, dest, size);
			dest++;
			size--;
			break;

		case 's':
			s = va_arg(ap, const char *);

			n = put(s, strlen(s), dest, size);
			dest += n;
			size -= n;
			break;

		case 'l':	// ignore
			goto parse;

		case 'p':
			n = put("0x", 2, dest, size);
			dest += n;
			size -= n;
			// fall through
		case 'X':
		case 'x':
			base = 16;
			goto number;

		case 'd':
		case 'u':
			base = 10;
			goto number;

		case 'b':
			base = 2;
			goto number;

		number:
			if (prec == 0)
				prec = 8;
			n = va_arg(ap, unsigned int);

			utostr(buf, prec, n, base);
			n = put(buf, prec, dest, size);
			dest += n;
			size -= n;
			break;

		case '0':	// FIXME: pad with zeroes
			goto parse;

		case '1'...'9':
			prec = prec * 10 + c - '0';
			goto parse;

		default:
			break;
		}
	}
	if (idx) {
		n = put(fmt - idx - 1, idx, dest, size);
		dest += n;
		size -= n;
	}

	return dest - begin;
}
