// SPDX-License-Identifier: GPL-2.0-only
/*
 * Very simple but very effective user-space memory tester.
 * Originally by Simon Kirby <sim@stormix.com> <sim@neato.org>
 * Version 2 by Charles Cazabon <charlesc-memtester@pyropus.ca>
 * Version 3 not publicly released.
 * Version 4 rewrite:
 * Copyright (C) 2004-2012 Charles Cazabon <charlesc-memtester@pyropus.ca>
 * Licensed under the terms of the GNU General Public License version 2 (only).
 */

#include <memtester.h>
#include <prng.h>

static const char progress[] = "-\\|/";
#define PROGRESSLEN 4

typedef unsigned long ul;
typedef unsigned long volatile ulv;
typedef unsigned char volatile u8v;
typedef unsigned short volatile u16v;


#define putchar(c)	printf("%c", c)
#define	rand_ul()	prng()
#define	UL_LEN		(sizeof(unsigned long) * 8)
#define	UL_BYTE(x)	(((~0UL) / 0xff) * (x))
#define CHECKERBOARD	((~0UL) / 3)


static int compare_regions(ulv *bufa, ulv *bufb, ulong count)
{
	ulv *p1 = bufa;
	ulv *p2 = bufb;
	int r = 0;

	for (ulong i = 0; i++ < count; ) {
		unsigned long v1 = *p1++, v2 = *p2++;
		if (v1 == v2)
			continue;
		pr_err("%08lx != %08lx @ %p / %p\n", v1, v2, p1, p2);
		r = 1;
	}
	return r;
}


static int test_stuck_address(ulv *bufa, ulv *bufb, ulong count)
{
	ulv *p1 = bufa;
	uint j;

	count *= 2;			// size of bufa + bufb

	putchar(' ');
	for (j = 0; j < 16; j++) {
		p1 = bufa;
		for (ulong i = 0; i < count; i++) {
			ul val = ((j + i) % 2) == 0 ? (ul) p1 : ~((ul) p1);
			*p1++ = val;
		}
		p1 = bufa;
		for (ulong i = 0; i < count; i++) {
			ul val = ((j + i) % 2) == 0 ? (ul) p1 : ~((ul) p1);
			if (*p1++ == val)
				continue;
			pr_err("FAILURE: possible bad address line @ %p\n", p1);
			return -1;
		}
		putchar('\b');
		putchar(progress[j % PROGRESSLEN]);
	}
	putchar('\b');
	return 0;
}

static int test_random_value(ulv *bufa, ulv *bufb, ulong count)
{
	ulv *p1 = bufa;
	ulv *p2 = bufb;

	for (ulong i = 0; i < count; i++) {
		*p1++ = *p2++ = rand_ul();
	}
	return compare_regions(bufa, bufb, count);
}

static int test_xor_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	ulv *p1 = bufa;
	ulv *p2 = bufb;
	ul q = rand_ul();

	for (ulong i = 0; i < count; i++) {
		*p1++ ^= q;
		*p2++ ^= q;
	}
	return compare_regions(bufa, bufb, count);
}

static int test_sub_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	ulv *p1 = bufa;
	ulv *p2 = bufb;
	ul q = rand_ul();

	for (ulong i = 0; i < count; i++) {
		*p1++ -= q;
		*p2++ -= q;
	}
	return compare_regions(bufa, bufb, count);
}

static int test_mul_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	ulv *p1 = bufa;
	ulv *p2 = bufb;
	ul q = rand_ul();

	for (ulong i = 0; i < count; i++) {
		*p1++ *= q;
		*p2++ *= q;
	}
	return compare_regions(bufa, bufb, count);
}

static int test_div_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	ulv *p1 = bufa;
	ulv *p2 = bufb;
	ul q = rand_ul();

	for (ulong i = 0; i < count; i++) {
		if (!q) {
			q++;
		}
		*p1++ /= q;
		*p2++ /= q;
	}
	return compare_regions(bufa, bufb, count);
}

static int test_or_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	ulv *p1 = bufa;
	ulv *p2 = bufb;
	ul q = rand_ul();

	for (ulong i = 0; i < count; i++) {
		*p1++ |= q;
		*p2++ |= q;
	}
	return compare_regions(bufa, bufb, count);
}

static int test_and_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	ulv *p1 = bufa;
	ulv *p2 = bufb;
	ul q = rand_ul();

	for (ulong i = 0; i < count; i++) {
		*p1++ &= q;
		*p2++ &= q;
	}
	return compare_regions(bufa, bufb, count);
}

static int test_seqinc_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	ulv *p1 = bufa;
	ulv *p2 = bufb;
	ul q = rand_ul();

	for (ulong i = 0; i < count; i++) {
		*p1++ = *p2++ = (i + q);
	}
	return compare_regions(bufa, bufb, count);
}

static int test_solidbits_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	putchar(' ');
	for (int j = 0; j < 64; j++) {
		ul q = (j % 2) == 0 ? ~0UL : 0UL;
		ulv *p1 = bufa;
		ulv *p2 = bufb;

		for (ulong i = 0; i < count; i++) {
			*p1++ = *p2++ = q;
			q = ~q;
		}
		if (compare_regions(bufa, bufb, count)) {
			return -1;
		}
		putchar('\b');
		putchar(progress[j % PROGRESSLEN]);
	}
	putchar('\b');
	return 0;
}

static int test_checkerboard_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	putchar(' ');
	for (int j = 0; j < 64; j++) {
		ul q = (j % 2) == 0 ? CHECKERBOARD : ~CHECKERBOARD;
		ulv *p1 = bufa;
		ulv *p2 = bufb;

		for (ulong i = 0; i < count; i++) {
			*p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
		}
		if (compare_regions(bufa, bufb, count)) {
			return -1;
		}
		putchar('\b');
		putchar(progress[j % PROGRESSLEN]);
	}
	putchar('\b');
	return 0;
}

static int test_blockseq_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	putchar(' ');
	for (int j = 0; j < 256; j++) {
		ulv *p1 = bufa;
		ulv *p2 = bufb;

		for (ulong i = 0; i < count; i++) {
			*p1++ = *p2++ = UL_BYTE(j);
		}
		if (compare_regions(bufa, bufb, count)) {
			return -1;
		}
		putchar('\b');
		putchar(progress[j % PROGRESSLEN]);
	}
	putchar('\b');
	return 0;
}

static int test_walkbits0_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	putchar(' ');
	for (ulong j = 0; j < UL_LEN * 2; j++) {
		ulv *p1 = bufa;
		ulv *p2 = bufb;

		for (ulong i = 0; i < count; i++) {
			if (j < UL_LEN) {	/* Walk it up. */
				*p1++ = *p2++ = 1UL << j;
			} else {		/* Walk it back down. */
				*p1++ = *p2++ = 1UL << (UL_LEN * 2 - j - 1);
			}
		}
		if (compare_regions(bufa, bufb, count)) {
			return -1;
		}
		putchar('\b');
		putchar(progress[j % PROGRESSLEN]);
	}
	putchar('\b');
	return 0;
}

static int test_walkbits1_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	putchar(' ');
	for (ulong j = 0; j < UL_LEN * 2; j++) {
		ulv *p1 = bufa;
		ulv *p2 = bufb;

		for (ulong i = 0; i < count; i++) {
			if (j < UL_LEN) {	/* Walk it up. */
				*p1++ = *p2++ = ~(1UL << j);
			} else {		/* Walk it back down. */
				*p1++ = *p2++ = ~(1UL << (UL_LEN * 2 - j - 1));
			}
		}
		if (compare_regions(bufa, bufb, count)) {
			return -1;
		}
		putchar('\b');
		putchar(progress[j % PROGRESSLEN]);
	}
	putchar('\b');
	return 0;
}

static int test_bitspread_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	putchar(' ');
	for (ulong j = 0; j < UL_LEN * 2; j++) {
		ulv *p1 = bufa;
		ulv *p2 = bufb;

		for (ulong i = 0; i < count; i++) {
			if (j < UL_LEN) {	/* Walk it up. */
				*p1++ = *p2++ = (i % 2 == 0)
					? (1UL << j) | (1UL << (j + 2))
					: ~((1UL << j) | (1UL << (j + 2)));
			} else {		/* Walk it back down. */
				*p1++ = *p2++ = (i % 2 == 0)
					? (1UL << (UL_LEN * 2 - 1 - j)) | (1UL << (UL_LEN * 2 + 1 - j))
					: ~(1UL << (UL_LEN * 2 - 1 - j) | (1UL << (UL_LEN * 2 + 1 - j)));
			}
		}
		if (compare_regions(bufa, bufb, count)) {
			return -1;
		}
		putchar('\b');
		putchar(progress[j % PROGRESSLEN]);
	}
	putchar('\b');
	return 0;
}

static int test_bitflip_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	putchar(' ');
	for (ulong j = 0; j < (UL_LEN * 8); j++) {
		ul q = 1UL << (j % UL_LEN);
		ulv *p1 = bufa;
		ulv *p2 = bufb;

		for (ulong i = 0; i < count; i++) {
			*p1++ = *p2++ = q;
			q = ~q;
		}
		if (compare_regions(bufa, bufb, count)) {
			return -1;
		}
		putchar('\b');
		putchar(progress[j % PROGRESSLEN]);
	}
	putchar('\b');
	return 0;
}

static int test_8bit_wide_random(ulv *bufa, ulv *bufb, ulong count)
{
	putchar(' ');
	for (int j = 0; j < 2; j++) {
		u8v *p1;
		ulv *p2;

		if (j & 1) {
			p1 = (u8v *) bufa;
			p2 = bufb;
		} else {
			p1 = (u8v *) bufb;
			p2 = bufa;
		}
		for (ulong i = 0; i < count; i++) {
			ul q = rand_ul();
			u8 *src = (void*) &q;
			*p2++ = q;
			for (ulong b = 0; b < UL_LEN / 8; b++) {
				*p1++ = *src++;
			}
		}
		if (compare_regions(bufa, bufb, count)) {
			return -1;
		}
		putchar('\b');
		putchar(progress[j % PROGRESSLEN]);
	}
	putchar('\b');
	return 0;
}

static int test_16bit_wide_random(ulv *bufa, ulv *bufb, ulong count)
{
	putchar(' ');
	for (int j = 0; j < 2; j++) {
		u16v *p1;
		ulv *p2;

		if (j & 1) {
			p1 = (u16v *) bufa;
			p2 = bufb;
		} else {
			p1 = (u16v *) bufb;
			p2 = bufa;
		}
		for (ulong i = 0; i < count; i++) {
			ul q = rand_ul();
			u16 *src = (void*) &q;
			*p2++ = q;
			for (ulong h = 0; h < UL_LEN / 16; h++) {
				*p1++ = *src++;
			}
		}
		if (compare_regions(bufa, bufb, count)) {
			return -1;
		}
		putchar('\b');
		putchar(progress[j % PROGRESSLEN]);
	}
	putchar('\b');
	return 0;
}

static int test_selftest(ulv *bufa, ulv *bufb, ulong count)
{
#if 0
	return 1;
#endif
	return 0;
}


static const struct test {
	const char *name;
	int (*test)(ulv *bufa, ulv *bufb, ulong count);
} tests[] = {
	[MEMTESTER_SEQ_INC_BIT] = { "Sequential Inc.",	test_seqinc_comparison, },
	[MEMTESTER_RANDVAL_BIT] = { "Random Value",	test_random_value, },
	[MEMTESTER_COMPXOR_BIT] = { "Compare XOR",	test_xor_comparison, },
	[MEMTESTER_COMPSUB_BIT] = { "Compare SUB",	test_sub_comparison, },
	[MEMTESTER_COMPMUL_BIT] = { "Compare MUL",	test_mul_comparison, },
	[MEMTESTER_COMPDIV_BIT] = { "Compare DIV",	test_div_comparison, },
	[MEMTESTER_COMP_OR_BIT] = { "Compare OR",	test_or_comparison, },
	[MEMTESTER_COMPAND_BIT] = { "Compare AND",	test_and_comparison, },
	[MEMTESTER_WRITE8_BIT]  = { "8-bit Writes",	test_8bit_wide_random, },
	[MEMTESTER_WRITE16_BIT] = { "16-bit Writes",	test_16bit_wide_random, },
	[MEMTESTER_STUCKAD_BIT] = { "Stuck Address",	test_stuck_address, },
	[MEMTESTER_SOLBITS_BIT] = { "Solid Bits",	test_solidbits_comparison, },
	[MEMTESTER_CHECKER_BIT] = { "Checkerboard",	test_checkerboard_comparison, },
	[MEMTESTER_WALK1_BIT]   = { "Walking Ones",	test_walkbits1_comparison, },
	[MEMTESTER_WALK0_BIT]   = { "Walking Zeroes",	test_walkbits0_comparison, },
	[MEMTESTER_BIT_SPR_BIT] = { "Bit Spread",	test_bitspread_comparison, },
	[MEMTESTER_BLK_SEQ_BIT] = { "Block Sequential",	test_blockseq_comparison, },
	[MEMTESTER_BITFLIP_BIT] = { "Bit Flip",		test_bitflip_comparison, },
	{ "Selftest",	test_selftest },
	{ NULL }
};

int memtester(void *buff, ulong size, uint loops, uint testmask)
{
	ulv *bufa, *bufb;
	ulong count;
	int err = 0;
	uint i;

	printf("Memtester: %p - %p, loops: %d, tests: %x\n", buff, buff + size -1, loops, testmask);

	// split the buffer into two parts, each of <count> words.
	size /= 2;
	bufa = buff;
	bufb = buff + size;
	count = size / sizeof(ul);

	// Run test loops
	if (!loops)			// unlimited
		loops = ~0;
	// Which tests?
	if (!testmask)			// all tests
		testmask = ~0;
	for (uint loop = 0; loop < loops; loop++) {
		printf(" Loop %lu:\n", loop);

		for (i = 0; i < MEMTESTER_LAST_BIT; i++) {
			int rc;

			if (!tests[i].name)
				break;
			if ((testmask & (1 << i)) == 0)
				continue;

			printf("  %-16s: ", tests[i].name);
			rc = tests[i].test(bufa, bufb, count);
			if (rc == 0) {
				printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
			} else {
				printf("KO\n");
				err++;
			}
		}
		printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
		printf("                     ");
		printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
	}

	printf("Memtester: errors: %d\n", err);
	return !!err;
}
