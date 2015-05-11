#include <arch/assembly.h>
#include <mach/platform_setup.S>


	.section .entry,"a"
	.global __entry_vector
__entry_vector:
	.long	CONFIG_VIRT_ADDR + CONFIG_PHYS_SIZE
	.long	__startup
	.long	vec_nmi
	.long	vec_hardfault
	.size __entry_vector, . - __entry_vector
	.weak _os_startup
	.thumb_set _os_startup, __entry_vector


	.section .entry.text,"ax"
	.thumb_func
vec_nmi:
	b	vec_nmi
	.size   vec_nmi, .-vec_nmi

	.thumb_func
vec_hardfault:
	b	vec_hardfault
	.size   vec_hardfault, .-vec_hardfault


	.section .init.text,"ax"
	.type   __startup, %function
	.thumb_func
__startup:
	b	_os_start
	.size   __startup, .-__startup
