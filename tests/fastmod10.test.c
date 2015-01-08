#include <kapi/kapi.h>
#include <utils.h>
#include <string.h>


static const struct vector {
	unsigned long val;
} vectors[] = {
	{ 10, },
	{ 100, },
	{ 1000, },
	{ 10000, },
	{ 100000,},
	{ 1000000,},
	{ 10000000,},
	{ 100000000,},
	{ 1000000000,},
	{ 0x10000000, },
	{ 0x20000000, },
	{ 0x40000000, },
	{ 0x80000000, },
	{ 0x90000000, },
	{ 0x98000000, },
	{ 0x9FFFFFFF, },
	{ 0xA0000000, },
	{ 0xC0000000, },
	{ 0xF0000000, },
	{ 0xFF000000, },
	{ 0xFFF00000, },
	{ 0xFFFF0000, },
	{ 0xFFFFF000, },
	{ 0xFFFFFF00, },
	{ 0xFFFFFFF0, },
	{ 0xFFFFFFFF, },
};


struct divmod {
	unsigned long mod;
	unsigned long div;
};

static struct divmod meth1(unsigned long val)
{
	unsigned long tmp;
	struct divmod res;

	tmp = val / 10U;
	res.mod = val - (tmp * 10);
	res.div = tmp;
	return res;
}

static struct divmod meth2(unsigned long val)
{
	unsigned long tmp;
	struct divmod res;

	tmp = (val * 0xCCCCCCCDLL) >> 35;		// !!! 64 bits multiply
	res.mod = val - (tmp * 10U);
	res.div = tmp;
	return res;
}

static struct divmod meth21(unsigned long val)
{
	unsigned long tmp;
	struct divmod res;

	if (val >= 0xA0000000)
		tmp = val / 10U;
	else
		tmp = (val * 0xCCCCCCCDLL) >> 35;		// !!! 64 bits multiply
	res.mod = val - (tmp * 10);
	res.div = tmp;
	return res;
}

static struct divmod meth22(unsigned long val)
{
	unsigned long long ltmp;
	unsigned long tmp;
	struct divmod res;

	ltmp = (val * 0xCCCCCCCDLL);		// GCC bug workaround for: tmp = (val * 0xCCCCCCCDLL) >> 35;
	tmp = ltmp >> 35;
	res.mod = val - (tmp * 10U);
	res.div = tmp;
	return res;
}

static struct divmod meth3(unsigned long val)
{
	unsigned long tmp;
	struct divmod res;

	if (val < 81920)
		tmp = (val * 0xCCCD) >> 19;
	else
		tmp = (val * 0xCCCCCCCDLL) >> 35;	// !!! 64 bits multiply
	res.mod = val - (tmp * 10);
	res.div = tmp;
	return res;
}

#define test_meth(m, val, mod, div) \
	res = meth ## m(val);	\
	if (res.mod != mod || res.div != div) { \
		printf("#ERROR:\tmeth%u(%lx) should be %lx:%lx but is %lx:%lx\n", m, val, mod, div, res.mod, res.div); \
		ko++; \
	} else ok++

static int dotest(void)
{
	unsigned int ok = 0;
	unsigned int ko = 0;
	unsigned int i, j;

	for (i = 0; i < ARRAY_SIZE(vectors); i++) {
		const struct vector *v = &vectors[i];

		for (j = -10 ; j != 10; j++) {
			unsigned long val = v->val + j;
			unsigned long mod = val % 10U;
			unsigned long div = val / 10U;
			struct divmod res;
	
			test_meth(1, val, mod, div);
			test_meth(2, val, mod, div);
			test_meth(21,val, mod, div);
			test_meth(3, val, mod, div);
		}
	}

	printf("Tests fail: %u, succeed: %u\n", ko, ok);
	return ko;
}

void kapi_start(void)
{
	dotest();
}
