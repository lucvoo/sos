#include <statistics.h>
#include <math.h>

void statistics_init(struct statistics *s)
{
	s->val1 = s->n = 0;
	s->val2 = 0;
}

void statistics_add(struct statistics *s, unsigned long val)
{
	s->val1	+= val;
	s->val2 += val*val;
	s->n	+= 1;
}


unsigned long statistics_total(const struct statistics *s)
{
	return s->val1;
}

unsigned long statistics_n(const struct statistics *s)
{
	return s->n;
}

unsigned long statistics_mean(const struct statistics *s)
{
	return s->val1 / s->n;
}

unsigned long statistics_sdev(const struct statistics *s)
{
	unsigned n = s->n;
	unsigned long long d = n * s->val2 - s->val1*s->val1;

printf("N=	%lu\n", s->n);
printf("T1=	%lu\n", s->val1);
printf("T2=	0x%08lx%08lx\n", (unsigned long)(s->val2 >> 32), (unsigned long) s->val2);
printf("D2=	0x%08lx%08lx\n", (unsigned long)(d >> 32), (unsigned long) d);
printf("D1=	%lu\n", sqrtll(d));
printf("sd=	%lu\n", sqrtll(d)/n);

	return sqrtll(d)/n;
}
