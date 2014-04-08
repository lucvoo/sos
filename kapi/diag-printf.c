#include <diag.h>
#include <stdarg.h>
#include <string.h>
#include <irqflags.h>

static void utostr(char *buf, unsigned size, unsigned val, unsigned base)
{
	unsigned digit;
	int i;

	buf[size] = '\0';
	for (i = size; --i >= 0;) {
		digit = val % base;
		val /= base;
		if (digit > 9)
			digit += 'a' - 10;
		else
			digit += '0';

		buf[i] = digit;
	}
}

static void _diag_vprintf(const char *fmt, va_list ap)
{
	unsigned long irqflags = __local_irq_save();
	unsigned int c;
	unsigned prec;
	int idx;

	idx = 0;
	while ((c = (unsigned int)*fmt++)) {
		if (c != '%') {
			idx++;
			continue;
		}
		if (idx) {
			_os_diag_write_buf(fmt - idx - 1, idx);
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
			_os_diag_write_buf(buf, 1);
			break;

		case 'B':
		case 's':
			s = va_arg(ap, const char *);
			if (!s)
				s = "<nil>";
			if (c == 'B')
				n = va_arg(ap, unsigned int);
			else
				n = strlen(s);

			_os_diag_write_buf(s, n);
			break;

		case 'l':	// ignore
			goto parse;

		case 'p':
			_os_diag_write_buf("0x", 2);
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
			_os_diag_write_buf(buf, prec);
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
		_os_diag_write_buf(fmt - idx - 1, idx);
	}

        __local_irq_rest(irqflags);
}

void _os_diag_printf(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	_diag_vprintf(fmt, ap);
	va_end(ap);
}
