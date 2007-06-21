#define	DECLARE_STACK(name, size)	\
	extern unsigned longname[];	\
	unsigned long name[size/sizeof(unsigned long)] __attribute__((aligned(CONFIG_STACK_ALIGNMENT)))

DECLARE_STACK(__interrupt_stack, CONFIG_INTERRUPT_STACK_SIZE);

DECLARE_STACK(__exception_stack, CONFIG_EXCEPTION_STACK_SIZE);

DECLARE_STACK(__startup_stack, CONFIG_STARTUP_STACK_SIZE);
