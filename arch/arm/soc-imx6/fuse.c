#include <soc/fuse.h>
#include <soc/baseaddr.h>
#include <init.h>
#include <io.h>


static void imx_fuse_init(void)
{
	void __iomem *iobase;

	iobase = ioremap(OCOTP_BASE, OCOTP_SIZE);

}
board_setup_initcall(imx_fuse_init);
