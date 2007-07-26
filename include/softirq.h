#ifndef	_SOFTIRQ_H_
#define	_SOFTIRQ_H_

enum {
	SOFTIRQ_DSR,

	SOFTIRQ_MAX,
};


struct softirq_action {
	void (*action)(struct softirq_action *);
	void *data;
};

int softirq_register(unsigned int nr, void (*action)(struct softirq_action*), void* data);

#endif
