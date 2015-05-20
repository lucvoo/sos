#include <kapi/kapi.h>
#include <hexdump.h>
#include <string.h>

#define	ALIGN	8
#define	GRANUL	16
#define	TAIL	(ALIGN*2)

#define LENGTH (ALIGN + GRANUL + TAIL)


#define	TEST(COND)	if (!(COND)) { err++; binhex(dst, sizeof(dst)); printf("doff: %u, soff: %u: n = %u\n", doff, soff, n); goto end_test; }


static unsigned char dst[LENGTH] __aligned(ALIGN);
static unsigned char src[LENGTH] __aligned(ALIGN);

static int test_memcpy(void)
{
	unsigned int i;
	unsigned int doff;
	unsigned int soff;
	unsigned int n;
	unsigned int err = 0;
	unsigned int cnt = 0;

	for (i = 0; i < sizeof(src); i++) {
		src[i] = i;
	}

	for (doff = 0; doff < ALIGN; doff++) {
		for (soff = 0; soff < ALIGN; soff++) {
			for (n = 0; n < GRANUL; n++) {
				const unsigned char *ptr;

				// clear the destination buffer
				for (i = 0; i < sizeof(dst); i++) {
					dst[i] = 0xff;
				}

				memcpy(dst + doff, src + soff, n);

				ptr = &dst[0];
				for (i = 0; i < doff; i++)
					TEST(ptr[i] == 0xff);

				ptr += doff;
				for (i = 0; i < n; i++)
					TEST(ptr[i] == soff + i);

				ptr += n;
				for (i = 0; i < TAIL; i++)
					TEST(ptr[i] == 0xff);

end_test:
				cnt++;
			}
		}
	}

	printf("%s: failed %d/%d\n", __func__, err, cnt);
	return err != 0;;
}

void kapi_start(void)
{
	test_memcpy();
}
