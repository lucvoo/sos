/*
 * Used by asm code
 */


void __div0(void);

void __div0(void)
{
	// FIXME: printf("Division by zero!\n");
}

void abort(void);
void abort(void)
{
	printf("abort!\n");
	for (;;) ;
	// FIXME:
}
