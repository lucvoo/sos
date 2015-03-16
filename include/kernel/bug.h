#ifndef _KERNEL_BUG_H_
#define _KERNEL_BUG_H_


struct eframe;

void __noreturn __bug(const struct eframe *regs, const char *file, const char *func, int line);

#endif
