#include <kapi/kapi.h>
#include <memtester.h>


#define	ADDR	CONFIG_MEMTESTER_ADDR
#define	SIZE	CONFIG_MEMTESTER_SIZE

void kapi_start(void)
{
	memtester((void *)ADDR, SIZE, 32, MEMTESTER_ALL);
}
