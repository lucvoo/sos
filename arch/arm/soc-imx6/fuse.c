#include <soc/fuse.h>
#include <soc/baseaddr.h>
#include <string.h>
#include <init.h>
#include <io.h>
#include <net/macaddr.h>


static struct macaddr macaddr;

static void imx_fuse_init(void)
{
	void __iomem *iobase;
	u8 *mac = macaddr.byte;
	u32 val;

	iobase = ioremap(OCOTP_BASE, OCOTP_SIZE);

	// read the (first) mac address
	val = ioread32(iobase + 0x620);
	mac[5] = val >> 0;
	mac[4] = val >> 8;
	mac[3] = val >> 16;
	mac[2] = val >> 24;
	val = ioread32(iobase + 0x630);
	mac[1] = val >> 0;
	mac[0] = val >> 8;
}
board_setup_initcall(imx_fuse_init);

void imx_get_macaddr(struct macaddr *addr)
{
	macaddr_copy(addr, &macaddr);
}
