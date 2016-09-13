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

static char progress[] = "-\\|/";
#define PROGRESSLEN 4
#define PROGRESSOFTEN 2500


typedef unsigned long ul;
typedef unsigned long long ull;
typedef unsigned long volatile ulv;
typedef unsigned char volatile u8v;
typedef unsigned short volatile u16v;


#define putchar(c)	printf("%c", c)
#define	rand_ul()	prng()
#define	UL_LEN		CONFIG_BITS
#define	UL_BYTE(x)	(((~0UL) / 0xff) * (x))
#define CHECKERBOARD1	((~0UL) / 3)
#define CHECKERBOARD2	(~CHECKERBOARD1)


union {
	unsigned char bytes[UL_LEN / 8];
	ul val;
} mword8;

union {
	unsigned short u16s[UL_LEN / 16];
	ul val;
} mword16;

static int compare_regions(ulv *bufa, ulv *bufb, ulong count)
{
	int r = 0;
	ulong i;
	ulv *p1 = bufa;
	ulv *p2 = bufb;

	for (i = 0; i < count; i++, p1++, p2++) {
		if (*p1 != *p2) {
			pr_err("FAILURE: 0x%08lx != 0x%08lx at offset 0x%08lx.\n",
					(ul) *p1, (ul) *p2, (ul) (i * sizeof(ul)));
			/* printf("Skipping to next test..."); */
			r = -1;
		}
	}
	return r;
}


static int test_stuck_address(ulv *bufa, ulv *bufb, ulong count)
{
	ulv *p1 = bufa;
	uint j;
	ulong i;

	count *= 2;			// size of bufa + bufb

	printf("           ");
	for (j = 0; j < 16; j++) {
		printf("\b\b\b\b\b\b\b\b\b\b\b");
		p1 = (ulv *) bufa;
		printf("setting %3u", j);
		for (i = 0; i < count; i++) {
			*p1 = ((j + i) % 2) == 0 ? (ul) p1 : ~((ul) p1);
			*p1++;
		}
		printf("\b\b\b\b\b\b\b\b\b\b\b");
		printf("testing %3u", j);
		p1 = (ulv *) bufa;
		for (i = 0; i < count; i++, p1++) {
			if (*p1 != (((j + i) % 2) == 0 ? (ul) p1 : ~((ul) p1))) {
				pr_err("FAILURE: possible bad address line at offset "
						"0x%08lx.\n", (ul) (i * sizeof(ul)));
				printf("Skipping to next test...\n");
				return -1;
			}
		}
	}
	printf("\b\b\b\b\b\b\b\b\b\b\b           \b\b\b\b\b\b\b\b\b\b\b");
	return 0;
}

static int test_random_value(ulv *bufa, ulv *bufb, ulong count)
{
	ulv *p1 = bufa;
	ulv *p2 = bufb;
	ul j = 0;
	ulong i;

	putchar(' ');
	for (i = 0; i < count; i++) {
		*p1++ = *p2++ = rand_ul();
		if (!(i % PROGRESSOFTEN)) {
			putchar('\b');
			putchar(progress[++j % PROGRESSLEN]);
		}
	}
	printf("\b \b");
	return compare_regions(bufa, bufb, count);
}

static int test_xor_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	ulv *p1 = bufa;
	ulv *p2 = bufb;
	ulong i;
	ul q = rand_ul();

	for (i = 0; i < count; i++) {
		*p1++ ^= q;
		*p2++ ^= q;
	}
	return compare_regions(bufa, bufb, count);
}

static int test_sub_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	ulv *p1 = bufa;
	ulv *p2 = bufb;
	ulong i;
	ul q = rand_ul();

	for (i = 0; i < count; i++) {
		*p1++ -= q;
		*p2++ -= q;
	}
	return compare_regions(bufa, bufb, count);
}

static int test_mul_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	ulv *p1 = bufa;
	ulv *p2 = bufb;
	ulong i;
	ul q = rand_ul();

	for (i = 0; i < count; i++) {
		*p1++ *= q;
		*p2++ *= q;
	}
	return compare_regions(bufa, bufb, count);
}

static int test_div_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	ulv *p1 = bufa;
	ulv *p2 = bufb;
	ulong i;
	ul q = rand_ul();

	for (i = 0; i < count; i++) {
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
	ulong i;
	ul q = rand_ul();

	for (i = 0; i < count; i++) {
		*p1++ |= q;
		*p2++ |= q;
	}
	return compare_regions(bufa, bufb, count);
}

static int test_and_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	ulv *p1 = bufa;
	ulv *p2 = bufb;
	ulong i;
	ul q = rand_ul();

	for (i = 0; i < count; i++) {
		*p1++ &= q;
		*p2++ &= q;
	}
	return compare_regions(bufa, bufb, count);
}

static int test_seqinc_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	ulv *p1 = bufa;
	ulv *p2 = bufb;
	ulong i;
	ul q = rand_ul();

	for (i = 0; i < count; i++) {
		*p1++ = *p2++ = (i + q);
	}
	return compare_regions(bufa, bufb, count);
}

static int test_solidbits_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	ulv *p1 = bufa;
	ulv *p2 = bufb;
	uint j;
	ul q;
	ulong i;

	printf("           ");
	for (j = 0; j < 64; j++) {
		printf("\b\b\b\b\b\b\b\b\b\b\b");
		q = (j % 2) == 0 ? ~0UL : 0;
		printf("setting %3u", j);
		p1 = (ulv *) bufa;
		p2 = (ulv *) bufb;
		for (i = 0; i < count; i++) {
			*p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
		}
		printf("\b\b\b\b\b\b\b\b\b\b\b");
		printf("testing %3u", j);
		if (compare_regions(bufa, bufb, count)) {
			return -1;
		}
	}
	printf("\b\b\b\b\b\b\b\b\b\b\b           \b\b\b\b\b\b\b\b\b\b\b");
	return 0;
}

static int test_checkerboard_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	ulv *p1 = bufa;
	ulv *p2 = bufb;
	uint j;
	ul q;
	ulong i;

	printf("           ");
	for (j = 0; j < 64; j++) {
		printf("\b\b\b\b\b\b\b\b\b\b\b");
		q = (j % 2) == 0 ? CHECKERBOARD1 : CHECKERBOARD2;
		printf("setting %3u", j);
		p1 = (ulv *) bufa;
		p2 = (ulv *) bufb;
		for (i = 0; i < count; i++) {
			*p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
		}
		printf("\b\b\b\b\b\b\b\b\b\b\b");
		printf("testing %3u", j);
		if (compare_regions(bufa, bufb, count)) {
			return -1;
		}
	}
	printf("\b\b\b\b\b\b\b\b\b\b\b           \b\b\b\b\b\b\b\b\b\b\b");
	return 0;
}

static int test_blockseq_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	ulv *p1 = bufa;
	ulv *p2 = bufb;
	uint j;
	ulong i;

	printf("           ");
	for (j = 0; j < 256; j++) {
		printf("\b\b\b\b\b\b\b\b\b\b\b");
		p1 = (ulv *) bufa;
		p2 = (ulv *) bufb;
		printf("setting %3u", j);
		for (i = 0; i < count; i++) {
			*p1++ = *p2++ = (ul) UL_BYTE(j);
		}
		printf("\b\b\b\b\b\b\b\b\b\b\b");
		printf("testing %3u", j);
		if (compare_regions(bufa, bufb, count)) {
			return -1;
		}
	}
	printf("\b\b\b\b\b\b\b\b\b\b\b           \b\b\b\b\b\b\b\b\b\b\b");
	return 0;
}

static int test_walkbits0_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	ulv *p1 = bufa;
	ulv *p2 = bufb;
	uint j;
	ulong i;

	printf("           ");
	for (j = 0; j < UL_LEN * 2; j++) {
		printf("\b\b\b\b\b\b\b\b\b\b\b");
		p1 = (ulv *) bufa;
		p2 = (ulv *) bufb;
		printf("setting %3u", j);
		for (i = 0; i < count; i++) {
			if (j < UL_LEN) {	/* Walk it up. */
				*p1++ = *p2++ = 1UL << j;
			} else {	/* Walk it back down. */
				*p1++ = *p2++ = 1UL << (UL_LEN * 2 - j - 1);
			}
		}
		printf("\b\b\b\b\b\b\b\b\b\b\b");
		printf("testing %3u", j);
		if (compare_regions(bufa, bufb, count)) {
			return -1;
		}
	}
	printf("\b\b\b\b\b\b\b\b\b\b\b           \b\b\b\b\b\b\b\b\b\b\b");
	return 0;
}

static int test_walkbits1_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	ulv *p1 = bufa;
	ulv *p2 = bufb;
	uint j;
	ulong i;

	printf("           ");
	for (j = 0; j < UL_LEN * 2; j++) {
		printf("\b\b\b\b\b\b\b\b\b\b\b");
		p1 = (ulv *) bufa;
		p2 = (ulv *) bufb;
		printf("setting %3u", j);
		for (i = 0; i < count; i++) {
			if (j < UL_LEN) {	/* Walk it up. */
				*p1++ = *p2++ = (~0UL) ^ (1UL << j);
			} else {	/* Walk it back down. */
				*p1++ = *p2++ = (~0UL) ^ (1UL << (UL_LEN * 2 - j - 1));
			}
		}
		printf("\b\b\b\b\b\b\b\b\b\b\b");
		printf("testing %3u", j);
		if (compare_regions(bufa, bufb, count)) {
			return -1;
		}
	}
	printf("\b\b\b\b\b\b\b\b\b\b\b           \b\b\b\b\b\b\b\b\b\b\b");
	return 0;
}

static int test_bitspread_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	ulv *p1;
	ulv *p2;
	uint j;
	ulong i;

	printf("           ");
	for (j = 0; j < UL_LEN; j++) {
		ulong msk = (1UL << j) | (1UL << (j + 2));

		printf("\b\b\b\b\b\b\b\b\b\b\b");
		p1 = bufa;
		p2 = bufb;
		printf("setting %3u", j);
		for (i = 0; i < count; i++) {
			/* Walk it up. */
			*p1++ = *p2++ = (i % 2 == 0) ?  msk : ~msk;
		}
		printf("\b\b\b\b\b\b\b\b\b\b\b");
		printf("testing %3u", j);
		if (compare_regions(bufa, bufb, count))
			return -1;
	}
	for (j = UL_LEN; j--; ) {
		ulong msk = (1UL << j) | (1UL << (j + 2));

		printf("\b\b\b\b\b\b\b\b\b\b\b");
		p1 = bufa;
		p2 = bufb;
		printf("setting %3u", j);
		for (i = 0; i < count; i++) {
			/* Walk it back down. */
			*p1++ = *p2++ = (i % 2 == 0) ?  msk : ~msk;
		}
		printf("\b\b\b\b\b\b\b\b\b\b\b");
		printf("testing %3u", j);
		if (compare_regions(bufa, bufb, count)) {
			return -1;
		}
	}
	printf("\b\b\b\b\b\b\b\b\b\b\b           \b\b\b\b\b\b\b\b\b\b\b");
	return 0;
}

static int test_bitflip_comparison(ulv *bufa, ulv *bufb, ulong count)
{
	ulv *p1 = bufa;
	ulv *p2 = bufb;
	uint j, k;
	ul q;
	ulong i;

	printf("           ");
	for (k = 0; k < UL_LEN; k++) {
		q = 1UL << k;
		for (j = 0; j < 8; j++) {
			printf("\b\b\b\b\b\b\b\b\b\b\b");
			q = ~q;
			printf("setting %3u", k * 8 + j);
			p1 = (ulv *) bufa;
			p2 = (ulv *) bufb;
			for (i = 0; i < count; i++) {
				*p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
			}
			printf("\b\b\b\b\b\b\b\b\b\b\b");
			printf("testing %3u", k * 8 + j);
			if (compare_regions(bufa, bufb, count)) {
				return -1;
			}
		}
	}
	printf("\b\b\b\b\b\b\b\b\b\b\b           \b\b\b\b\b\b\b\b\b\b\b");
	return 0;
}

static int test_8bit_wide_random(ulv *bufa, ulv *bufb, ulong count)
{
	u8v *p1, *t;
	ulv *p2;
	int attempt;
	uint b, j = 0;
	ulong i;

	putchar(' ');
	for (attempt = 0; attempt < 2; attempt++) {
		if (attempt & 1) {
			p1 = (u8v *) bufa;
			p2 = bufb;
		} else {
			p1 = (u8v *) bufb;
			p2 = bufa;
		}
		for (i = 0; i < count; i++) {
			t = mword8.bytes;
			*p2++ = mword8.val = rand_ul();
			for (b = 0; b < UL_LEN / 8; b++) {
				*p1++ = *t++;
			}
			if (!(i % PROGRESSOFTEN)) {
				putchar('\b');
				putchar(progress[++j % PROGRESSLEN]);
			}
		}
		if (compare_regions(bufa, bufb, count)) {
			return -1;
		}
	}
	printf("\b \b");
	return 0;
}

static int test_16bit_wide_random(ulv *bufa, ulv *bufb, ulong count)
{
	u16v *p1, *t;
	ulv *p2;
	int attempt;
	uint b, j = 0;
	ulong i;

	putchar(' ');
	for (attempt = 0; attempt < 2; attempt++) {
		if (attempt & 1) {
			p1 = (u16v *) bufa;
			p2 = bufb;
		} else {
			p1 = (u16v *) bufb;
			p2 = bufa;
		}
		for (i = 0; i < count; i++) {
			t = mword16.u16s;
			*p2++ = mword16.val = rand_ul();
			for (b = 0; b < UL_LEN / 16; b++) {
				*p1++ = *t++;
			}
			if (!(i % PROGRESSOFTEN)) {
				putchar('\b');
				putchar(progress[++j % PROGRESSLEN]);
			}
		}
		if (compare_regions(bufa, bufb, count)) {
			return -1;
		}
	}
	printf("\b \b");
	return 0;
}


struct memtest {
	const char *name;
	int (*test)(ulv *bufa, ulv *bufb, ulong count);
};

static struct memtest tests[] = {
	[MEMTESTER_STUCKAD_BIT] = { "Stuck Address",	test_stuck_address, },
	[MEMTESTER_RANDVAL_BIT] = { "Random Value",	test_random_value, },
	[MEMTESTER_COMPXOR_BIT] = { "Compare XOR",	test_xor_comparison, },
	[MEMTESTER_COMPSUB_BIT] = { "Compare SUB",	test_sub_comparison, },
	[MEMTESTER_COMPMUL_BIT] = { "Compare MUL",	test_mul_comparison, },
	[MEMTESTER_COMPDIV_BIT] = { "Compare DIV",	test_div_comparison, },
	[MEMTESTER_COMP_OR_BIT] = { "Compare OR",	test_or_comparison, },
	[MEMTESTER_COMPAND_BIT] = { "Compare AND",	test_and_comparison, },
	[MEMTESTER_SEQ_INC_BIT] = { "Sequential Inc.",	test_seqinc_comparison, },
	[MEMTESTER_SOLBITS_BIT] = { "Solid Bits",	test_solidbits_comparison, },
	[MEMTESTER_BLK_SEQ_BIT] = { "Block Sequential",	test_blockseq_comparison, },
	[MEMTESTER_CHECKER_BIT] = { "Checkerboard",	test_checkerboard_comparison, },
	[MEMTESTER_BIT_SPR_BIT] = { "Bit Spread",	test_bitspread_comparison, },
	[MEMTESTER_BITFLIP_BIT] = { "Bit Flip",		test_bitflip_comparison, },
	[MEMTESTER_WALK1_BIT]   = { "Walking Ones",	test_walkbits1_comparison, },
	[MEMTESTER_WALK0_BIT]   = { "Walking Zeroes",	test_walkbits0_comparison, },
	[MEMTESTER_WRITE8_BIT]  = { "8-bit Writes",	test_8bit_wide_random, },
	[MEMTESTER_WRITE16_BIT] = { "16-bit Writes",	test_16bit_wide_random, },
};

int memtester(void *buff, ulong size, int loops, uint testmask)
{
	void *bufa, *bufb;
	ulong count;
	int err = 0;
	int rc;
	uint i;

	printf("Memtester: %p - %p, loops: %d, tests: %x\n", buff, buff + size -1, loops, testmask);

	// split the buffer into two parts, each of <count> words.
	size /= 2;
	bufa = buff;
	bufb = buff + size;
	count = size / sizeof(ulong);

	do {
		printf("  Loop %d:\n", loops);

		for (i = 0; i < MEMTESTER_LAST_BIT; i++) {
			if ((testmask & (1 << i)) == 0)
				continue;

			printf("  %-20s: ", tests[i].name);
			rc = tests[i].test(bufa, bufb, count);
			if (rc == 0)
				printf("ok\n");
			else
				err++;
		}
	} while (--loops > 0);

	printf("Memtester: errors: %d\n", err);
	return !!err;
}
