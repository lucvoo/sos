	.section .init.text,"ax"
	.global	_os_start
	.type   _os_start, %function
	.thumb_func
_os_start:
	b	_os_start

	.size _os_start, .-_os_start
