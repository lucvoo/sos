#include <init.h>


void _os_initcalls(void)
{
	extern struct initcall __INITCALL_LIST__[];
	struct initcall *p;

	for (p = &__INITCALL_LIST__[0]; p->fun; p++) {
		p->fun();
	}
}
