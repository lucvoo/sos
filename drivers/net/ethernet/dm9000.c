#include <net/dev.h>
#include "dm9000.h"
#include <delay.h>
#include <errno.h>
#include <io.h>
#include <irqdesc.h>
#include <init.h>


struct dm9000 {
	struct netdev		ndev;

	void __iomem*		iodata;
};


/******************************************************************************/
static inline unsigned int dm9000_ior(struct dm9000 *dev, int reg)
{
	iowrite8(dev->ndev.iobase, reg);
	return ioread8(dev->iodata);
}

static inline void dm9000_iow(struct dm9000 *dev, int reg, int val)
{
	iowrite8(dev->ndev.iobase, reg);
	iowrite8(dev->iodata, val);
}

/******************************************************************************/
static unsigned int dm9000_read_id(struct dm9000 *dev)
{
	unsigned int id;

	id = 0;
	id |= dm9000_ior(dev, DM9000_PIDH) << 24;
	id |= dm9000_ior(dev, DM9000_PIDL) << 16;
	id |= dm9000_ior(dev, DM9000_VIDH) <<  8;
	id |= dm9000_ior(dev, DM9000_VIDL) <<  0;

	return id;
}

static int dm9000_reset_once(struct dm9000 *dev)
{
	dm9000_iow(dev, DM9000_NCR, NCR_LBK_MAC|NCR_RST);
	udelay(100);			// at least 20us

	return dm9000_ior(dev, DM9000_NCR) &  NCR_RST;
}

static void dm9000_reset(struct dm9000 *dev)
{
	// Rem. need a double reset and set LBK into normal MAC mode

	// First try
	if (dm9000_reset_once(dev))
		pr_info("did not reset (try 1)\n");

	// second try
	dm9000_iow(dev, DM9000_NCR, 0);
	if (dm9000_reset_once(dev))
		pr_warn("did not reset (try 2)\n");
}

/******************************************************************************/
// FIXME: use something like platform_device or device tree ?
struct dm9000_cfg {
	unsigned long	iobase;
	unsigned long	iodata;
	const char*	intctrl;
	unsigned int	irq;
};

static int dm9000_probe(struct dm9000 *dev, const struct dm9000_cfg *cfg)
{
	void __iomem*		iobase;
	void __iomem*		iodata;
	struct irqdesc*		irq;
	unsigned int id;
	int rc = -ENODEV;

	iobase = ioremap(cfg->iobase, 2);
	iodata = ioremap(cfg->iodata, 4);

	irq = irq_get_desc(cfg->intctrl, cfg->irq);
	if (!irq) {
		pr_err("no such irq: %s(%d)\n", cfg->intctrl, cfg->irq);
		// FIXME: goto ...
		return -ENODEV;
	}

	dev->ndev.iobase = iobase;
	dev->ndev.irq = irq;
	dev->iodata = iodata;

	dm9000_reset(dev);

	/* check ID */
	id = dm9000_read_id(dev);
	if (id != DM9000_ID) {
		pr_err("wrong ID: %08x\n", id);
		goto end;
	}

	/* check model/type */
	id = dm9000_ior(dev, DM9000_CHIPR);
	pr_info("model = %02x\n", id);
	switch (id) {
	// FIXME: some model can do hardware checksum
	case CHIPR_A:
		break;
	case CHIPR_B:
		break;
	default:
		break;
	}

	// TODO: add netdev ops
	// TODO: add "ethtool" ops
	// TODO: add MII

	// TODO: read MAC address from eeprom

	rc = netdev_register(&dev->ndev);
	return rc;

end:
	// FIXME: release ressources
	return rc;
}

static struct dm9000 dm9000_dev;

static void dm9000_init(void)
{
#define	DM9000_ENTRY(I, B, IC, IX)	{ I, B, IC, IX, },
	const struct dm9000_cfg dm9000_cfg[1] = {
#include <mach/config-dm9000.h>
	};

	dm9000_probe(&dm9000_dev, dm9000_cfg);
}
device_initcall(dm9000_init);
