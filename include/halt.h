#ifndef _HALT_H_
#define _HALT_H_


struct eframe;

void __noreturn __halt(const struct eframe *regs);

#endif
