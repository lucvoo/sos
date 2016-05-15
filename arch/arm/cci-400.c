#include <arch/hw/cci-400.h>
#include <arch/cci-400.h>
#include <soc/cci-400.h>
#include <arch/iomap.h>
#include <arch/cp15.h>
#include <io.h>


static void cci400_enable_interface(void __iomem *base, uint interface)
{
	ulong snoop_ctrl = CCI400_SNOOP_CTRL(interface);
	u32 status;
	u32 ctrl;

	ctrl = ioread32(base + snoop_ctrl) & CCI400_ALL_ENABLE;
	if (ctrl  == CCI400_ALL_ENABLE)
		return;			// already enabled

	// enable it
	iowrite32(base + snoop_ctrl, ctrl | CCI400_ALL_ENABLE);

	// wait for changes to complete
	do {
		status = ioread32(base + CCI400_STATUS);
	} while (status & CCI400_STATUS_PENDING);
}

void __cci400_enable(void)
{
	void __iomem *base = __ioremap(CCI400_BASE);
	u32 mpidr = cp_read(MPIDR);
	u32 cluster = MPIDR_CLUSTER(mpidr);
	int interface = cci400_interface(cluster);

	if (interface < 0)
		return;

	cci400_enable_interface(base, interface);
}
