#include <arch/arch.h>
#include <thread.h>

#define DEFINE(sym, val) \
        asm volatile("\n->" #sym " %0 " #val "<-" : : "i" (val))

#define BLANK() asm volatile("\n->" : : )

int main(void)
{
  DEFINE(TI_CPU_SAVE,		offsetof(struct thread, cpu_context));
  BLANK();
  DEFINE(S_R0,			offsetof(struct eframe, r0));
  DEFINE(S_R1,			offsetof(struct eframe, r1));
  DEFINE(S_R2,			offsetof(struct eframe, r2));
  DEFINE(S_R3,			offsetof(struct eframe, r3));
  DEFINE(S_R4,			offsetof(struct eframe, r4));
  DEFINE(S_R5,			offsetof(struct eframe, r5));
  DEFINE(S_R6,			offsetof(struct eframe, r6));
  DEFINE(S_R7,			offsetof(struct eframe, r7));
  DEFINE(S_R8,			offsetof(struct eframe, r8));
  DEFINE(S_R9,			offsetof(struct eframe, r9));
  DEFINE(S_R10,			offsetof(struct eframe, r10));
  DEFINE(S_FP,			offsetof(struct eframe, fp));
  DEFINE(S_IP,			offsetof(struct eframe, ip));
  DEFINE(S_SP,			offsetof(struct eframe, sp));
  DEFINE(S_LR,			offsetof(struct eframe, lr));
  DEFINE(S_PC,			offsetof(struct eframe, pc));
  DEFINE(S_PSR,			offsetof(struct eframe, cpsr));
  DEFINE(S_OLD_R0,		offsetof(struct eframe, old_r0));
  DEFINE(S_FRAME_SIZE,		sizeof(struct eframe));
  BLANK();
  return 0;
}
