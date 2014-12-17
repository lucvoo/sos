#include <diag.h>

void diag_abort(unsigned long far, unsigned fsr, int type, unsigned long pc);

void diag_abort(unsigned long far, unsigned fsr, int type, unsigned long pc)
{
	_os_diag_printf("%c@%lx: FAR=%lx, FSR=%lx\n", type, pc, far, fsr);

	while (1)
		;
}
