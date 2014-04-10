#ifndef	_PERF_H_
#define	_PERF_H_

void perf_cycle_counter_start(void);
void perf_cycle_counter_stop(void);
void perf_cycle_counter_reset(void);
unsigned long perf_cycle_counter_get(void);

#endif
