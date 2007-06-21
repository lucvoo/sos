void __clear_bss(void)
{
	extern unsigned long __bss_start;
	extern unsigned long __bss_end;
	unsigned long* ptr = &__bss_start;
	unsigned long* end = &__bss_end;

	while (ptr < end) {
		*ptr++ = 0;
	}
}
