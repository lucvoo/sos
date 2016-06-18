#include <prng.h>
#include <symbols.h>


/*
 * Reference:
 *	"An experimental exploration of Marsaglia’s xorshift generators",
 *	Sebastiano Vigna
 *
 * Principle:
 *	Some bit mixing with the xorshift, followed by another scrambling
 *	done by an inversible multiplication (here just an odd number).
 *	Since the result of the multiplication is not stored, the period
 *	is just the period of the xorshifts (2^64 - 1 for the 64bit version).
 */

u64 prng64(void)
{
	static u64 x = 0x3243F6A8885A308D;	// 3.243F6A8885A308D3... is pi in hex

	// xorshift*:
	// - must be seeded with a nonzero value
	// - maximal period of (1 << 64) - 1
	// - pass most tests (but BigCrush's MatrixRank)
	x ^= x >> 12;
	x ^= x << 25;
	x ^= x >> 27;

	return x * 0x2545F4914F6CDD1D;		// M32
}
#if CONFIG_BITS == 64
strong_alias(prng64, prng);
#endif


u32 prng32(void)
{
	static u32 x = 0x3243F6A8;		// 3.243F6A8885A308D3... is pi in hex

	// 32 bit version of the above
	// - must be seeded with a nonzero value
	x ^= x >> 12;
	x ^= x << 25;
	x ^= x >> 27;

	return x * 0x4F6CDD1D;			// M32 mod 32
}
#if CONFIG_BITS == 32
strong_alias(prng32, prng);
#endif
