#include <kapi/kapi.h>
#include <version.h>
#include <sched.h>
#include <dma-mapping.h>


static void dma_write(void)
{
	static u8 buff[256] __aligned(256);
	ulong addr;

	addr = dma_wbuff_map(buff, sizeof(buff));

	// insure the CPU can safely access the buffer
	dma_wbuff_get(addr, sizeof(buff));
	// Fill the buffer
	dma_wbuff_put(addr, sizeof(buff));

	// do DMA stuff
}

static void dma_read(void)
{
	static u8 buff[256] __aligned(256);
	ulong addr;

	addr = dma_rbuff_map(buff, sizeof(buff));

	// insure the device can safely write to the buffer
	dma_rbuff_put(addr, sizeof(buff));

	// do DMA stuff
	// and wait for the result

	// insure the CPU can safely access the buffer
	dma_rbuff_get(addr, sizeof(buff));
}

void kapi_start(void)
{

	dma_write();

	dma_read();
}
