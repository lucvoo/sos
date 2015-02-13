#include <thread.h>

#define DEFINE(sym, val) \
        asm volatile("\n->" #sym " %0 " #val "<-" : : "i" (val))

#define BLANK() asm volatile("\n->" : : )

int main(void)
{
	DEFINE(THR_CPU_SREGS,		offsetof(struct thread, cpu_context.s));
	DEFINE(THR_CPU_SP,		offsetof(struct thread, cpu_context.sp));
	DEFINE(THR_CPU_FP,		offsetof(struct thread, cpu_context.fp));
	DEFINE(THR_CPU_RA,		offsetof(struct thread, cpu_context.ra));
	BLANK();
	return 0;
}
