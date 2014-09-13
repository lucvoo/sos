#ifndef _STATISTICS_H_
#define _STATISTICS_H_


struct statistics {
	unsigned long		val1;
	unsigned long long	val2;
	unsigned long		n;
};

void statistics_init(struct statistics *s);
void statistics_add(struct statistics *s, unsigned long val);

unsigned long statistics_total(const struct statistics *s);
unsigned long statistics_mean(const struct statistics *s);
unsigned long statistics_n(const struct statistics *s);
unsigned long statistics_sdev(const struct statistics *s);

#endif
