#include <arch/arch.h>
#include <thread.h>
#include <arch/exceptions.h>

#define DEFINE(sym, val) \
        asm volatile("\n->" #sym " %0 " #val "<-" : : "i" (val))

#define BLANK() asm volatile("\n->" : : )

int main(void)
{
  DEFINE(TI_CONTEXT_X0,		offsetof(struct thread, cpu_context.x[0]));
  DEFINE(THR_CTXT_SP,		offsetof(struct thread, cpu_context.sp));
  BLANK();
  DEFINE(EFRAME_REGS,		offsetof(struct eframe, x[0]));
  DEFINE(EFRAME_LR,		offsetof(struct eframe, x[30]));
  DEFINE(EFRAME_SP,		offsetof(struct eframe, sp));
  DEFINE(EFRAME_PC,		offsetof(struct eframe, pc));
  DEFINE(EFRAME_PSTATE,		offsetof(struct eframe, pstate));
  DEFINE(EFRAME_SIZE,		sizeof(struct eframe));
  BLANK();
  return 0;
}
