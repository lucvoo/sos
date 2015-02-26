#ifndef	_KAPI_H_
#define	_KAPI_H_

void kapi_start(void);

/* kernel/kapi.c */
void kapi_scheduler_start(void);


#ifdef	CONFIG_SMP
void kapi_start_smp(void);
#endif

#endif
