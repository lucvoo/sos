#include <kapi/kapi.h>
#include <utils.h>
#include <bitops/findbit.h>


struct vector {
	unsigned long	val;
	int		res;
};


static const struct vector vectors_ffs[] = {
	{ 0,			0, }, //8*sizeof(unsigned long), },
	{ ~0UL,			1, },
	{ 1 << 0,		1+0, },
	{ 1 << 1,		1+1, },
	{ 1 << 2,		1+2, },
	{ 1 << 31,		1+31, },
};

static const struct vector vectors_ctz[] = {
	//{ 0,			unspecified, },
	{ ~0UL,			0, },
	{ 1 << 0,		0, },
	{ 1 << 1,		1, },
	{ 1 << 2,		2, },
	{ 1 << 31,		31, },
};

static const struct vector vectors_clz[] = {
	//{ 0,			unspecified, },
	{ ~0UL,			0, },
	{ ~0UL >> 1,		1, },
	{ ~0UL >> 2,		2, },
	{ ~0UL >> 31,		31, },
};


static int dotest(const struct vector *vec, unsigned n, int (*op)(unsigned long), const char *name)
{
	unsigned int ok = 0;
	unsigned int ko = 0;
	unsigned int i;

	for (i = 0; i < n; i++) {
		const struct vector *v = &vec[i];
		int r = op(v->val);

		if (r != v->res) {
			printf("#ERROR:\t%s(%lu) should be %lu but is %lu\n", name, v->val, v->res, r);
			ko++;
		} else
			ok++;
	}

	printf("Tests %s(): fail: %u, succeed: %u\n", name, ko, ok);
	return ko;
}

#define	FIND(name)	static int name(unsigned long val) { return bitop_ ##name(val); }

FIND(ffs)
FIND(ctz)
FIND(clz)

#define	DOTEST(name)	dotest(vectors_ ## name, ARRAY_SIZE(vectors_ ## name), name, #name)

void kapi_start(void)
{
	DOTEST(ffs);
	DOTEST(ctz);
	DOTEST(clz);
}
