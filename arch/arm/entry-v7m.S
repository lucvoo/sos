#include <arch/assembly.h>
#include <mach/platform_setup.S>


	.section .entry,"a"
	.global __entry_vector
__entry_vector:
	.long	CONFIG_VIRT_ADDR + CONFIG_PHYS_SIZE
	.long	__startup
	.long	vec_nmi
	.long	vec_hardfault
	.long	vec_mmfault
	.long	vec_busfault
	.long	vec_usagefault
	.long	vec_reserved
	.long	vec_reserved
	.long	vec_reserved
	.long	vec_reserved
	.long	vec_svc
	.long	vec_reserved
	.long	vec_reserved
	.long	vec_pendsv
	.long	vec_systick
	.rept	CONFIG_NVIC_NBR_IRQ
	.long	vec_irq
	.endr
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

	.thumb_func
vec_mmfault:
	b	vec_mmfault
	.size   vec_mmfault, .-vec_mmfault

	.thumb_func
vec_busfault:
	b	vec_busfault
	.size   vec_busfault, .-vec_busfault

	.thumb_func
vec_usagefault:
	b	vec_usagefault
	.size   vec_usagefault, .-vec_usagefault

	.thumb_func
vec_svc:
	b	vec_svc
	.size   vec_svc, .-vec_svc

	.thumb_func
vec_pendsv:
	b	vec_pendsv
	.size   vec_pendsv, .-vec_pendsv

	.thumb_func
vec_systick:
	b	vec_systick
	.size   vec_systick, .-vec_systick

	.thumb_func
vec_irq:
	b	vec_irq
	.size   vec_irq, .-vec_irq

	.thumb_func
vec_reserved:
	b	vec_reserved
	.size   vec_reserved, .-vec_reserved


	.section .init.text,"ax"
	.type   __startup, %function
	.thumb_func
__startup:
#ifdef CONFIG_BOOT
	BOOT_SETUP
#endif
	b	_os_start
	.size   __startup, .-__startup
