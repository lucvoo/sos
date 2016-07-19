#include <arch/arch.h>
#include <thread.h>
#include <arch/exceptions.h>

#define DEFINE(sym, val) \
        asm volatile("\n->" #sym " %0 " #val "<-" : : "i" (val))

#define BLANK() asm volatile("\n->" : : )

int main(void)
{
  DEFINE(TI_CPU_SAVE,		offsetof(struct thread, cpu_context));
  DEFINE(THR_CTXT_SP,		offsetof(struct thread, cpu_context.sp));
  BLANK();
  DEFINE(S_SP,			offsetof(struct eframe, sp));
  DEFINE(S_LR,			offsetof(struct eframe, lr));
  DEFINE(S_PC,			offsetof(struct eframe, pc));
  DEFINE(S_PSR,			offsetof(struct eframe, cpsr));
#if CONFIG_ARM_ARCH < 6
  DEFINE(S_OLD_R0,		offsetof(struct eframe, old_r0));
#endif
  DEFINE(S_FRAME_SIZE,		sizeof(struct eframe));
  BLANK();
  return 0;
}
