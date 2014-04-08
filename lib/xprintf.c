#include "xprintf.h"
#include <printf.h>
#include <stdarg.h>
#include <string.h>


#include <diag.h>

#define	F_CARD	0x01
#define	F_ZERO	0x02
#define	F_SPAC	0x04
#define	F_LEFT	0x08
#define	F_PLUS	0x10
#define	F_APOS	0x20
#define	F_UPPER	0x40

enum lmod {
	lm_none,
	lm_l,
	lm_ll,
};


static unsigned int utostr(char *buf, unsigned prec, unsigned long val, unsigned int base, unsigned int flags)
{
	unsigned int n;
	unsigned int ten = 'a' - 10;

	if (flags & F_UPPER)
		ten = 'A' - 10;

	for (n= 0; val > 0 || n < prec; val /= base, n++) {
		unsigned int digit = val % base;

		if (digit > 9)
			digit += ten;
		else
			digit += '0';

		*--buf = digit;
	}

	return n;
}


static void pad(put_fn_t put, char *dest, unsigned size, unsigned int n, unsigned int flags)
{
	static const char spaces[] = "               ";
	static const char zeroes[] = "000000000000000";
	const char *pads = spaces;
	unsigned s;

	if (flags & F_ZERO)
		pads = zeroes;

	while (n) {
		s = sizeof(spaces) -1;
		if (s > n)
			s = n;
		put(pads, s, dest, size);
		n -= s;
	}
}

unsigned int xprintf(put_fn_t put, char *dest, unsigned size, const char *fmt, va_list ap)
{
	char *begin = dest;

	while (1) {
		char buff[sizeof(unsigned long)*8 +1];		// enough for output in base 2
		unsigned int minw;
		unsigned int prec;
		unsigned int idx;
		unsigned int n;
		unsigned int c;
		unsigned flags = 0;
		const char *s;
		int lmod;

		// 0) process the non-conversion parts
		for (idx = 0; c = fmt[idx], c != '\0' && c != '%'; )
			idx++;

		if (idx) {
			n = put(fmt, idx, dest, size);
			dest += n;
			size -= n;
			fmt += idx;
		}

		if (c == '\0')
			break;

		fmt++;			// skip the '%'

		if (fmt[0] == '%') {
			put(fmt, 1, dest, size);
			fmt++;
			dest++;
			size--;
			continue;
		}

		// we have a real format specifier
		lmod = lm_none;
		minw = 0;
		prec = ~0;

		// 1) flags;
		for ( ; (c = *fmt); fmt++) {
			switch (c) {
			case '#':	flags |= F_CARD; continue;
			case '0':	flags |= F_ZERO; continue;
			case ' ':	flags |= F_SPAC; continue;
			case '-':	flags |= F_LEFT; continue;
			case '+':	flags |= F_PLUS; continue;
			//case '\'':	flags |= F_APOS; continue;
			}
			break;
		}
		if (flags & F_LEFT)
			flags &= ~F_ZERO;
		if (flags & F_PLUS)
			flags &= ~F_SPAC;

		// 2) minimum field width
		for ( ; (c = *fmt); fmt++) {
			unsigned int d = c - '0';

			if (d > 9)
				break;

			minw = minw * 10 + d;
		}

		// 3) precision
		if (c == '.') {
			fmt++;
			prec = 0;
			for ( ; (c = *fmt); fmt++) {
				unsigned int d = c - '0';

				if (d > 9)
					break;

				prec = prec * 10 + d;
			}
		}

		// 4) length modifiers
		switch (c) {
		case 'h':			// promoted to int
			fmt++;
			if (*fmt == 'h') {
				fmt++;
			}
			break;

		case 'l':
			lmod = lm_l;
			fmt++;
			if (*fmt == 'l') {
				lmod = lm_ll;
				fmt++;
			}
			break;

		case 't':			// same as long
		case 'j':			// same as long
		case 'z':			// same as long
			lmod = lm_l;
			fmt++;
			break;
		}

		// 5) type specifier
		switch (c = *fmt) {
			char *buf;
			unsigned int base;
			unsigned long uval;
			signed long sval;
			int sign;
			int neg;

		case 'c':
			uval = va_arg(ap, unsigned int);

			buff[0] = uval;
			s = buff;
			n = 1;
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
			break;

		case 'd':
		case 'i':
			switch (lmod) {
			default:
			case lm_none:	sval = va_arg(ap, int);		break;
			case lm_l:	sval = va_arg(ap, long);	break;
			//case q_ll:	sval = va_arg(ap, long long);	break;
			}

			if (sval < 0) {
				neg = 1;
				uval = -sval;
			} else {
				neg = 0;
				uval = sval;
			}

			goto case_number;

		case 'u':
		case 'o':
		case 'x':
		case 'X':
		case 'b':
			switch (lmod) {
			default:
			case lm_none:	uval = va_arg(ap, unsigned int);	break;
			case lm_l:	uval = va_arg(ap, unsigned long);	break;
			//case q_ll:	uval = va_arg(ap, unsigned long long);	break;
			}
			neg = 0;

		case_number:
			if (prec == ~0U) {
				prec = 1;
			} else
				flags &= ~F_ZERO;

			buf = &buff[sizeof(buff)];

			switch (c) {
				unsigned int shift;

			// decimal numbers
			case 'd':
			case 'i':
			case 'u':
				base = 10;
				break;

			// bin, oct or hex numbers
			case 'b':
				shift = 1;
				goto base_bin;
			case 'o':
				shift = 3;
				goto base_bin;
			case 'p':
				put("0x", 2, dest, size);
				dest += 2;
				size -= 2;
				prec = sizeof(long) * 2;
				// fall through
			case 'X':
				flags |= F_UPPER;
			case 'x':
				shift = 4;
			base_bin:
				base = 1 << shift;
				break;

			default:
				continue;
			}

			if (neg)
				sign = '-';
			else if (flags & F_PLUS)
				sign = '+';
			else if (flags & F_SPAC)
				sign = ' ';
			else
				sign = 0;

			if (flags & F_ZERO) {
				prec = minw;
				if (sign && prec > 1)
					prec--;
				if (prec < 1)
					prec = 1;
			}
			n = utostr(buf, prec, uval, base, flags);
			buf -= n;
			if (sign) {
				*--buf = sign;
				n++;
			}

			s = buf;
			n = buff + sizeof(buff) - buf;
			break;

		default:
			continue;
		}

		if (n < minw && (flags & F_LEFT) == 0) {
			unsigned int p = minw - n;

			pad(put, dest, size, p, 0);
			dest += p;
			size -= p;
		}
		n = put(s, n, dest, size);
		dest += n;
		size -= n;
		if (n < minw && (flags & F_LEFT) != 0) {
			unsigned int p = minw - n;

			pad(put, dest, size, p, 0);
			dest += p;
			size -= p;
		}
		fmt++;
	}

	return dest - begin;
}
