	.syntax unified
	.section .init.text,"ax"
	.global	_os_start
	.type   _os_start, %function
	.thumb_func
_os_start:
	ldr	r0, =(CONFIG_BOOTADDR)
	ldr	sp, [r0, #0]
	ldr	pc, [r0, #4]

	.size _os_start, .-_os_start
