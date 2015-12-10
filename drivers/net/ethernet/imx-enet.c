#include <net/dev.h>
#include "imx-enet.h"
#include <errno.h>
#include <io.h>
#include <irqdesc.h>
#include <irq.h>
#include <init.h>
#include <interrupt.h>
#include <mii.h>
#include <net/ether.h>
#include <net/skb.h>
#include <round.h>
#include <clk.h>
#include <hz.h>
#include <soc/iomux.h>
#include <mach/pincfg.h>


struct enet {
	struct netdev		ndev;
	struct irqaction	irq_action;
	struct mii		mii;

	struct clk		*ipg;
	struct clk		*ahb;
	struct clk		*enet;
	struct clk		*ref;
	struct clk		*ptp;
};


static int enet_phy_wait(struct netdev *ndev, uint timeout)
{
	// FIXME: this should be done via the interrupt handler

	// wait for completion
	while (!(ioread32(ndev->iobase + ENET_EIR) & EIR_MII)) {
		// FIXME
		// if timeout return error
	}
	// clear event
	iowrite32(ndev->iobase + ENET_EIR, EIR_MII);
	return 0;
}

static int enet_phy_read(struct mii *mii, uint phy, uint reg)
{
	struct netdev *ndev = mii->dev;
	void __iomem *io = ndev->iobase;
	uint val;
	int rc;

	iowrite32(io + ENET_EIR, EIR_MII);
	iowrite32(io + ENET_MMFR, MMFR_READ(phy, reg));

	rc = enet_phy_wait(ndev, HZ / 1000);
	if (rc)
		return rc;

	// ok: read the value
	val = ioread32(io + ENET_MMFR) & 0xFFFF;
	return val;
}


static int enet_phy_write(struct mii *mii, uint phy, uint reg, uint val)
{
	struct netdev *ndev = mii->dev;
	void __iomem *io = ndev->iobase;
	int rc;

	iowrite32(io + ENET_EIR, EIR_MII);
	iowrite32(io + ENET_MMFR, MMFR_WRITE(phy, reg, val));

	rc = enet_phy_wait(ndev, HZ / 1000);
	return rc;
}

/******************************************************************************/
static int enet_mii_init(struct enet *dev)
{
	void __iomem *iobase = dev->ndev.iobase;
	ulong ipg_rate;
	uint speed;
	uint hold;

	ipg_rate = clk_get_rate(dev->ipg);
	speed = DIV_ROUND_UP(ipg_rate, 5000000);
	if (speed > 64) {
		pr_err("invalid MII speed (IPG too fast: %lu)\n", ipg_rate);
		return -EINVAL;
	}

	// minimum of 10ns
	hold = DIV_ROUND_UP(ipg_rate, 100000000);
	iowrite32(iobase + ENET_MSCR, MSCR_SPEED(speed) | MSCR_HOLD(hold));
	return 0;
}

static void enet_reset(struct enet *dev)
{
	void __iomem *iobase = dev->ndev.iobase;
	u32 val;

	val = ioread32(iobase + ENET_ECR);
	iowrite32(iobase + ENET_ECR, val | ECR_RESET);

	iowrite32(iobase + ENET_ECR, val | ECR_ETHEREN);
}

static struct clk *clk_get_enable(const char *name)
{
	struct clk *clk = clk_get(name);

	if (clk)
		clk_enable(clk);

	return clk;
}

static void clk_disable_put(struct clk *clk)
{
	if (!clk)
		return;
	clk_disable(clk);
	clk_put(clk);
}

static void enet_clk_release(struct enet *dev)
{
	clk_disable_put(dev->ptp);
	clk_disable_put(dev->ref);
	clk_disable_put(dev->enet);
	clk_disable_put(dev->ahb);
	clk_disable_put(dev->ipg);
}

static int enet_clk_init(struct enet *dev)
{
	dev->ipg = clk_get_enable("ipg");
	dev->ahb = clk_get_enable("ahb");
	dev->enet= clk_get_enable("enet");
	dev->ref = clk_get_enable("enet_ref");
	dev->ptp = clk_get_enable("enet_ptp");

	if (!dev->ipg | !dev->ahb | !dev->enet | !dev->ref | !dev->ptp) {
		enet_clk_release(dev);
		return -EINVAL;
	}

	return 0;
}

static int enet_init_dev(struct enet *dev)
{
	struct mii *mii = &dev->mii;
	void __iomem *iobase;
	u32 phy_id;
	int rc;

#define	ENET_BASE	0x02188000
#define	ENET_SIZE	0x00000800
#define	ENET_IRQCHIP	"gic"
#define	ENET_IRQIDX	150
	iobase = ioremap(ENET_BASE, ENET_SIZE);
	if (!iobase) {
		rc = -EINVAL;
		goto err_ioremap;
	}
	dev->ndev.iobase = iobase;

	rc = enet_clk_init(dev);
	if (rc)
		goto err_clk;

	// TODO: add "ethtool" ops

	mii->read = enet_phy_read;
	mii->write = enet_phy_write;
	mii->dev = &dev->ndev;


	imx_iomux(imx_pincfg_enet1);
	enet_reset(dev);

	rc = enet_mii_init(dev);
	if (rc)
		goto err_mii;
	rc = mii_scan_phy(mii, &mii->paddr, &phy_id);
	pr_note("scan_phy(): rc = %d, paddr = %02x, id = %08x\n", rc, mii->paddr, phy_id);
	if (rc)
		goto err_phy;

err_phy:
err_mii:
	enet_clk_release(dev);
err_clk:
	//irq_put_desc(irq);
	iounmap(iobase, ENET_SIZE);
err_ioremap:
	return rc;
}

static struct enet enet;
static void enet_init(void)
{
	enet_init_dev(&enet);
}
device_initcall(enet_init);
