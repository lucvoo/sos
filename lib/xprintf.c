#include "xprintf.h"
#include <printf.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>


#include <diag.h>

#define	F_CARD	0x01
#define	F_ZERO	0x02
#define	F_SPAC	0x04
#define	F_LEFT	0x08
#define	F_PLUS	0x10
#define	F_APOS	0x20
#define	F_UPPER	0x40
#define	F_SEP8	0x80

enum lmod {
	lm_none,
	lm_l,
	lm_ll,
};


static uint utostr_bin(char *buf, uint prec, ulong val, uint shift, uint flags)
{
	uint ten = 'a' - 10;
	uint mask = (1 << shift) -1;
	char *endb = buf - prec;
	char *buf0 = buf;
	uint d;

	if (flags & F_UPPER)
		ten = 'A' - 10;

	for (d = 0; val > 0 || endb < buf; val >>= shift) {
		uint digit = val & mask;

		if ((flags & F_SEP8) && ((d % 8) == 0) && (d > 0))
			*--buf = '.';

		if (digit > 9)
			digit += ten;
		else
			digit += '0';

		*--buf = digit;
		d++;
	}

	return buf0 - buf;
}

static uint utostr_dec(char *buf, uint prec, ulong val)
{
	char *endb = buf - prec;
	char *buf0 = buf;

	for (; val > 0 || endb < buf;) {
		uint digit;
		uint tmp;

#define METH 2
#if	(METH == 0) || defined(CONFIG_HAS_DIV32)
		(void) tmp;

		digit = val % 10U;

		val /= 10U;
#elif	METH == 1
		tmp = val / 10U;
		digit = val - (tmp * 10);

		val = tmp;
#elif	METH == 2
		tmp = (val * 0xCCCCCCCDULL) >> 35;		// !!! 64 bits multiply
		digit = val - (tmp * 10);

		val = tmp;
#elif	METH == 3
		if (val < 81920)
			tmp = (val * 0xCCCD) >> 19;
		else
			tmp = (val * 0xCCCCCCCDULL) >> 35;	// !!! 64 bits multiply
		digit = val - (tmp * 10);

		val = tmp;
#endif

		digit += '0';
		*--buf = digit;
	}

	return buf0 - buf;
}


static void pad(struct xput *xput, uint n, uint flags)
{
	static const char spaces[] = "               ";
	static const char zeroes[] = "000000000000000";
	const char *pads = spaces;
	uint s;

	if (flags & F_ZERO)
		pads = zeroes;

	while (n) {
		s = sizeof(spaces) -1;
		if (s > n)
			s = n;
		xput->func(pads, s, xput);
		n -= s;
	}
}

static void pad_front(uint n, uint minw, uint flags, struct xput *xput)
{
	if ((n < minw) && (flags & F_LEFT) == 0)
		pad(xput, minw - n, 0);
}

static void pad_back(uint n, uint minw, uint flags, struct xput *xput)
{
	if ((n < minw) && (flags & F_LEFT) != 0)
		pad(xput, minw - n, 0);
}

/*****************************************************************************/
static uint xprintf(struct xput *xput, const char *fmt, ...)
{
	uint n;
	va_list ap;

	va_start(ap, fmt);
	n = xvprintf(xput, fmt, ap);
	va_end(ap);

	return n;
}

static int print_buf(struct xput *xput, const char *fmt, char sep, const uchar *p, uint len)
{
	uint n = 0;

	n += xprintf(xput, fmt, *p++);

	while (--len) {
		xput->func(&sep, 1, xput);
		n++;
		n += xprintf(xput, fmt, *p++);
	}

	return n;
}

static int print_macaddr(struct xput *xput, const uchar *p)
{
	return print_buf(xput, "%02x", ':', p, 6);
}

static int print_ipv4(struct xput *xput, const uchar *p)
{
	return print_buf(xput, "%d", '.', p, 4);
}

static uint print_binhex(struct xput *xput, const uchar *p, uint len)
{
	if (!len)
		return 0;

	return print_buf(xput, "%02x", ' ', p, len);
}

/*****************************************************************************/
uint xvprintf(struct xput *xput, const char *fmt, va_list ap)
{
	char *begin = xput->dest;

	while (1) {
		char buff[sizeof(ulong)*8 +8];		// enough for output in base 2 & seps
		uint minw;
		uint prec;
		uint idx;
		uint n;
		uint c;
		uint flags = 0;
		const char *s;
		int lmod;

		// 0) process the non-conversion parts
		for (idx = 0; c = fmt[idx], c != '\0' && c != '%'; )
			idx++;

		if (idx) {
			xput->func(fmt, idx, xput);
			fmt += idx;
		}

		if (c == '\0')
			break;

		fmt++;			// skip the '%'

		if (fmt[0] == '%') {
			xput->func(fmt, 1, xput);
			fmt++;
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
		if (*fmt == '*') {
			fmt++;
			minw = va_arg(ap, int);
			goto prec;
		}

		for ( ; (c = *fmt); fmt++) {
			uint d = c - '0';

			if (d > 9)
				break;

			minw = minw * 10 + d;
		}

prec:
		// 3) precision
		if (c == '.') {
			fmt++;
			if (*fmt == '*') {
				fmt++;
				prec = va_arg(ap, int);
				goto len_mods;
			}
			prec = 0;
			for ( ; (c = *fmt); fmt++) {
				uint d = c - '0';

				if (d > 9)
					break;

				prec = prec * 10 + d;
			}
			if (prec > sizeof(buff))
				prec = sizeof(buff);
		}

len_mods:
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
		switch (c = *fmt++) {
			char *buf;
			uint shift;
			ulong uval;
			long sval;
			int sign;
			int neg;

		case 'c':
			uval = va_arg(ap, uint);

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
				n = va_arg(ap, uint);
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

		case 'p': {
			const void *ptr = va_arg(ap, const void *);

			switch (fmt[0]) {
			case 'h':
				n = print_binhex(xput, ptr, minw);
				break;

#ifdef CONFIG_NET
			case 'M':
				n = print_macaddr(xput, ptr);
				break;

			case 'I':
				if (fmt[1] == '4') {	// IPv4
					fmt++;
					n = print_ipv4(xput, ptr);
					break;
				}
				/* fall-through */
#endif
			default:
				// FIXME: mess with pagging
				xput->func("0x", 2, xput);
				uval = (ulong) ptr;
				prec = sizeof(long) * 2;
				shift = 4;
				sign = 0;
				goto base_bin;
			}
			fmt++;
			// FIXME: no front padding
			// FIXME: no back  padding
			continue;

		}

		case 'u':
		case 'o':
		case 'x':
		case 'X':
		case 'b':
			switch (lmod) {
			default:
			case lm_none:	uval = va_arg(ap, uint);	break;
			case lm_l:	uval = va_arg(ap, ulong);	break;
			//case q_ll:	uval = va_arg(ap, unsigned long long);	break;
			}
			neg = 0;

		case_number:
			sign = 0;
			if (prec == ~0U) {
				prec = 1;
			} else
				flags &= ~F_ZERO;


			switch (c) {

			// decimal numbers
			case 'd':
			case 'i':
			case 'u':
				if (neg)
					sign = '-';
				else if (flags & F_PLUS)
					sign = '+';
				else if (flags & F_SPAC)
					sign = ' ';
				shift = 0;
				break;

			// bin, oct or hex numbers
			case 'b':
				if (flags & F_CARD)
					flags |= F_SEP8;
				shift = 1;
				goto base_bin;
			case 'o':
				shift = 3;
				goto base_bin;
			case 'X':
				flags |= F_UPPER;
			case 'x':
				shift = 4;
			base_bin:
				break;

			default:
				continue;
			}

			if (flags & F_ZERO) {
				prec = minw;
				if (sign && prec > 1)
					prec--;
				if (prec < 1)
					prec = 1;
			}

			buf = &buff[sizeof(buff)];
			if (!shift)
				n = utostr_dec(buf, prec, uval);
			else
				n = utostr_bin(buf, prec, uval, shift, flags);
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

		pad_front(n, minw, flags, xput);
		xput->func(s, n, xput);
		pad_back(n, minw, flags, xput);
	}

	return xput->dest - begin;
}
