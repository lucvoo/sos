#include <init.h>
#include <string.h>


void __init __high_vectors(void)
{
	extern void __vectors_start;
	extern void __vectors_end;

	memcpy((void*) 0xFFFF0000, &__vectors_start, &__vectors_end - &__vectors_start);
}
//pure_initcall(__high_vectors);
