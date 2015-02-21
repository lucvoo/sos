#include <thread.h>
#include <exceptions.h>

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

	DEFINE(EF_ZERO,			offsetof(struct frame, r[ 0]));
	DEFINE(EF_AT,			offsetof(struct frame, r[ 1]));
	DEFINE(EF_V0,			offsetof(struct frame, r[ 2]));
	DEFINE(EF_V1,			offsetof(struct frame, r[ 3]));
	DEFINE(EF_A0,			offsetof(struct frame, r[ 4]));
	DEFINE(EF_A1,			offsetof(struct frame, r[ 5]));
	DEFINE(EF_A2,			offsetof(struct frame, r[ 6]));
	DEFINE(EF_A3,			offsetof(struct frame, r[ 7]));
	DEFINE(EF_T0,			offsetof(struct frame, r[ 8]));
	DEFINE(EF_T1,			offsetof(struct frame, r[ 9]));
	DEFINE(EF_T2,			offsetof(struct frame, r[10]));
	DEFINE(EF_T3,			offsetof(struct frame, r[11]));
	DEFINE(EF_T4,			offsetof(struct frame, r[12]));
	DEFINE(EF_T5,			offsetof(struct frame, r[13]));
	DEFINE(EF_T6,			offsetof(struct frame, r[14]));
	DEFINE(EF_T7,			offsetof(struct frame, r[15]));
	DEFINE(EF_S0,			offsetof(struct frame, r[16]));
	DEFINE(EF_S1,			offsetof(struct frame, r[17]));
	DEFINE(EF_S2,			offsetof(struct frame, r[18]));
	DEFINE(EF_S3,			offsetof(struct frame, r[19]));
	DEFINE(EF_S4,			offsetof(struct frame, r[20]));
	DEFINE(EF_S5,			offsetof(struct frame, r[21]));
	DEFINE(EF_S6,			offsetof(struct frame, r[22]));
	DEFINE(EF_S7,			offsetof(struct frame, r[23]));
	DEFINE(EF_S8,			offsetof(struct frame, r[30]));
	DEFINE(EF_T8,			offsetof(struct frame, r[24]));
	DEFINE(EF_T9,			offsetof(struct frame, r[25]));
	DEFINE(EF_K0,			offsetof(struct frame, r[26]));
	DEFINE(EF_K1,			offsetof(struct frame, r[27]));
	DEFINE(EF_GP,			offsetof(struct frame, r[28]));
	DEFINE(EF_SP,			offsetof(struct frame, r[29]));
	DEFINE(EF_FP,			offsetof(struct frame, r[30]));
	DEFINE(EF_RA,			offsetof(struct frame, r[31]));
	DEFINE(EF_HI,			offsetof(struct frame, hi));
	DEFINE(EF_LO,			offsetof(struct frame, lo));

	DEFINE(EF_STATUS,		offsetof(struct frame, status));
	DEFINE(EF_CAUSE,		offsetof(struct frame, cause));
	DEFINE(EF_EPC,			offsetof(struct frame, epc));
	DEFINE(EF_SIZE,			sizeof(struct frame));
	return 0;
}
