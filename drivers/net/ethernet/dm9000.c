#include <net/dev.h>
#include "dm9000.h"
#include <delay.h>
#include <errno.h>
#include <io.h>
#include <irqdesc.h>
#include <irq.h>
#include <init.h>
#include <string.h>
#include <net/mii.h>
#include <interrupt.h>
#include <mii.h>
#include <byteorder.h>


struct dm9000 {
	struct netdev		ndev;

	void __iomem*		iodata;

	unsigned int		type;
#define	DM9000_TYPE_E		0
#define	DM9000_TYPE_A		1
#define	DM9000_TYPE_B		2

	unsigned char		imr;
	struct irqaction	irq_action;
	struct mii		mii;
};

#define	DM9000_PHYID	0x01	// Fixed and unique ID

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


static void dm9000_write_pkt(struct dm9000 *dev, const void *__src, unsigned int count)
{
	const unsigned char *src = __src;

	while (count--)
		iowrite8(dev->iodata, *src++);
}

static void dm9000_read_pkt(struct dm9000 *dev, void *__dst, unsigned int count)
{
	unsigned char *dst = __dst;

	while (count--)
		*dst++ = ioread8(dev->iodata);
}

static void dm9000_drop_pkt(struct dm9000 *dev, unsigned int count)
{
	while (count--)
		ioread8(dev->iodata);
}

/******************************************************************************/
static int dm9000_eeprom_wait(struct dm9000 *dev)
{
	int n;

	// poll the EPCR ERRE bit
	// timeout of 8000us
	for (n = 80; n; n--) {
		unsigned int epc = dm9000_ior(dev, DM9000_EPCR);

		if (!(epc & EPCR_ERRE))
			break;

		usleep(100);
	}

	return n;
}

/**
 * Read a 16bit word from the eeprom
 */
static void dm9000_eeprom_read(struct dm9000 *dev, unsigned char *dst, unsigned int offset)
{
	dm9000_iow(dev, DM9000_EPAR, offset);
	dm9000_iow(dev, DM9000_EPCR, EPCR_ERPRR);

	dm9000_eeprom_wait(dev);

	dm9000_iow(dev, DM9000_EPCR, 0);
	dst[0] = dm9000_ior(dev, DM9000_EPDRL);
	dst[1] = dm9000_ior(dev, DM9000_EPDRH);
}

static void dm9000_eeprom_get_macaddr(struct dm9000 *dev)
{
	unsigned char *addr = &dev->ndev.macaddr.byte[0];

	dm9000_eeprom_read(dev, &addr[0], 0);
	dm9000_eeprom_read(dev, &addr[2], 1);
	dm9000_eeprom_read(dev, &addr[4], 2);
}
/******************************************************************************/
static int dm9000_phy_read(struct mii *mii, unsigned int paddr, unsigned int reg)
{
	struct dm9000 *dev = container_of(mii->dev, struct dm9000, ndev);
	unsigned int val;

	// Set the PHY address and issue a PHY:READ command
	dm9000_iow(dev, DM9000_EPAR, EPAR_PADR(paddr) | reg);
	dm9000_iow(dev, DM9000_EPCR, EPCR_EPOS | EPCR_ERPRR);

	// FIXME: if we sleep here we need to save restore the current IO Register
	udelay(1000);

	// Clear the command
	dm9000_iow(dev, DM9000_EPCR, 0);

	// read the data from the PHY
	val = (dm9000_ior(dev, DM9000_EPDRH) << 8) | (dm9000_ior(dev, DM9000_EPDRL) << 0);

	return val;
}

static int dm9000_phy_write(struct mii *mii, unsigned int paddr, unsigned int reg, unsigned int val)
{
	struct dm9000 *dev = container_of(mii->dev, struct dm9000, ndev);

	// Write the data to the phy
	dm9000_iow(dev, DM9000_EPDRH, val >> 8);
	dm9000_iow(dev, DM9000_EPDRL, val >> 0);

	// Set the PHY address and issue a PHY:Write command
	dm9000_iow(dev, DM9000_EPAR, EPAR_PADR(paddr) | reg);
	dm9000_iow(dev, DM9000_EPCR, EPCR_EPOS | EPCR_ERPRW);

	// FIXME: if we sleep here we need to save restore the current IO Register
	udelay(1000);

	// Clear the command
	dm9000_iow(dev, DM9000_EPCR, 0);

	return 0;
}

/******************************************************************************/
static int dm9000_get_link(struct netdev *ndev)
{
	struct dm9000 *dev = container_of(ndev, struct dm9000, ndev);
	unsigned int status;

	// internal PHY
	status = dm9000_ior(dev, DM9000_NSR);
	return (status & NSR_LINKST) != 0;
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
static void dm9000_set_physical_address(struct dm9000 *dev)
{
	const unsigned char *macaddr = dev->ndev.macaddr.byte;
	int i;

	for (i = 0; i < 6; i++)
		dm9000_iow(dev, DM9000_PAR+i, macaddr[i]);
}

static void dm9000_reinit(struct dm9000 *dev)
{
	unsigned int ncr;
	unsigned int imr;

	dm9000_reset(dev);

	// Mask the interrupts
	dm9000_iow(dev, DM9000_IMR, IMR_PAR);

	// TODO: set hw checksum if capable

	// TODO: GPCR set all to output ?

	// Powerup & init the internal PHY
	dm9000_iow(dev, DM9000_GPR, GPR_PHYUP);
	if (dev->type == DM9000_TYPE_B) {
		dm9000_phy_write(&dev->mii, 0, MII_BMCR, MII_BMCR_RESET);
		dm9000_phy_write(&dev->mii, 0, MII_DM_DSPCR, MII_DSPCR_INIT_PARAM);
	}

	ncr = 0;
	// TODO: external PHY?
	// TODO: enable wake?
	dm9000_iow(dev, DM9000_NCR, ncr);

	// operating registers
	dm9000_iow(dev, DM9000_TCR, 0);	        // TX Polling clear
	dm9000_iow(dev, DM9000_BPTR, BPTR_BPHW(3)|BPTR_JPT_600US);
	dm9000_iow(dev, DM9000_FCR, 0xff);	// Flow Control
	dm9000_iow(dev, DM9000_SMCR, 0);        // Special Mode

	// clear TX status
	dm9000_iow(dev, DM9000_NSR, NSR_WAKEST | NSR_TX2END | NSR_TX1END);
	// Clear interrupt status
	dm9000_iow(dev, DM9000_ISR, ISR_CLR_STATUS);

	// write the MAc adress to ... the MAC
	dm9000_set_physical_address(dev);

	// RX enable
	dm9000_iow(dev, DM9000_RCR, RCR_RXEN);	// FIXME: |RCR_DIS_LONG|RCR_DIS_CRC);

	// TODO: Set address filter table

	// Save the interrupt mask
	imr = IMR_PAR | IMR_PTM | IMR_PRM;
	if (dev->type != DM9000_TYPE_E)
		imr |= IMR_LNKCHNG;
	dev->imr = imr;
}

/******************************************************************************/

static void dm9000_irq_tx(struct dm9000 *dev)
{
}

/******************************************************************************/

static void dm9000_irq_rx(struct dm9000 *dev)
{
	unsigned int ready;
	unsigned int len;
	struct rxhdr {
		u8	ready;	// 0: not ready, 1: ready, > 1: error?
		u8	status;
		__le16	length;
	} hdr;
	int err;

loop:
	// peek the ready byte from the header
	dm9000_ior(dev, DM9000_MRCMDX);
	ready = ioread8(dev->iodata);
	if (ready & DM9000_PKT_ERR) {
		pr_warn("RX ready error: %d\n", ready);
		dm9000_iow(dev, DM9000_RCR, 0x00);	// Stop Rx

		// FIXME: will need a reset in order to recover from
		//	  unstable states between system bus & DM9000 chip
		// FIXME: notify upper layer?
		return;
	}

	if (!ready)
		return;

	// now get the full header: with status & length
	iowrite8(dev->ndev.iobase, DM9000_MRCMD);
	dm9000_read_pkt(dev, &hdr, sizeof(hdr));
	len = le16_to_cpu(hdr.length);

	pr_dbg("RX: status = %02x, len = 0x%04x (%d)\n", hdr.status, len, len);

	// check length & status
	err = 0;
	if (len < 0x40) {
		err = 1;
		pr_err("RX: bad packet (len < 0x40)\n");
	}

	if (len > DM9000_PKT_MAX) {
		pr_dbg("RX: packet too long?\n");
	}

	// hdr.status is identical to RSR register.
	if (hdr.status & RSR_ERROR) {
		err = 1;
		if (hdr.status & RSR_FOE) {
			pr_dbg("RX: FIFO error\n");
		}
		if (hdr.status & RSR_CE) {
			pr_dbg("RX: CRC error\n");
		}
		if (hdr.status & RSR_RF) {
			pr_dbg("RX: length error\n");
		}
	}

	if (!err) {
		unsigned char dst[2048];

		// Read received packet from RX SRAM
		dm9000_read_pkt(dev, dst, len);
	}

	goto loop;
}

/******************************************************************************/

static int dm9000_irq(struct irqdesc *desc, void *data)
{
	struct dm9000 *dev = data;
	unsigned int saved_ioaddr;
	unsigned int status;
	int rc = IRQ_NONE;

	// save the ioaddr preset at interrupt time
	// and mask the MAC interrupts
	saved_ioaddr = ioread8(dev->ndev.iobase);
	dm9000_iow(dev, DM9000_IMR, IMR_PAR);

	// get the irq status and ack/clear it
	status = dm9000_ior(dev, DM9000_ISR);
	dm9000_iow(dev, DM9000_ISR, status);

	pr_dbg("status = %02x (%02x)\n", status, dm9000_ior(dev, DM9000_ISR));

	if (status & ISR_PRS) {
		dm9000_irq_rx(dev);
		rc |= IRQ_HANDLED;
	}
	if (status & ISR_PTS) {
		dm9000_irq_tx(dev);
		rc |= IRQ_HANDLED;
	}
	if (status & ISR_LNKCHNG) {
		rc |= IRQ_CALL_DSR;
	}

	// unmask the MAC interrupts
	// and restore the ioaddr
	dm9000_iow(dev, DM9000_IMR, dev->imr);
	iowrite8(dev->ndev.iobase, saved_ioaddr);

	return rc;
}

static int dm9000_dsr(struct irqdesc *desc, unsigned int count, void *data)
{
	struct dm9000 *dev = data;

	// FIXME: only save if this never sleep
	//	  which is true for now but ...
	mii_check_media(&dev->mii);

	return 0;
}

/******************************************************************************/

static int dm9000_open(struct netdev *ndev)
{
	struct dm9000 *dev = container_of(ndev, struct dm9000, ndev);
	int rc = 0;

	pr_info("enabling %s\n", ndev->ifname);

	// FIXME: why ???
	dm9000_iow(dev, DM9000_GPR, GPR_PHYUP);
	udelay(1000);

	dm9000_reinit(dev);

	irq_create(&dev->irq_action, dm9000_irq, dm9000_dsr, dev, 0);
	rc = irq_attach(ndev->irq, &dev->irq_action);
	irq_unmask(ndev->irq);

	// interrupt unmask
	dm9000_iow(dev, DM9000_IMR, dev->imr);

	mii_init_media(&dev->mii);

	return rc;
}

static int dm9000_poll(struct netdev *ndev)
{
	struct dm9000 *dev = container_of(ndev, struct dm9000, ndev);

	// For debug only

	mii_check_media(&dev->mii);

	return 0;
}
/******************************************************************************/
// FIXME: use something like platform_device or device tree ?
struct dm9000_cfg {
	unsigned long	iobase;
	unsigned long	iodata;
	const char*	intctrl;
	unsigned int	irq;
	const char*	macaddr;
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
		dev->type = DM9000_TYPE_A;
		break;
	case CHIPR_B:
		dev->type = DM9000_TYPE_B;
		break;
	default:
		dev->type = DM9000_TYPE_E;
		break;
	}

	// TODO: add netdev ops
	dev->ndev.open = dm9000_open;
	dev->ndev.poll = dm9000_poll;

	// TODO: add "ethtool" ops

	dev->mii.paddr = DM9000_PHYID;
	dev->mii.read = dm9000_phy_read;
	dev->mii.write = dm9000_phy_write;
	dev->mii.dev = &dev->ndev;

	dm9000_eeprom_get_macaddr(dev);
	if (!macaddr_is_valid(&dev->ndev.macaddr)) {
		memcpy(&dev->ndev.macaddr, cfg->macaddr, 6);
	}

	rc = netdev_register(&dev->ndev);
	return rc;

end:
	// FIXME: release ressources
	return rc;
}

static struct dm9000 dm9000_dev;

static void dm9000_init(void)
{
#define	DM9000_ENTRY(I, B, IC, IX, MA)	{ I, B, IC, IX, MA, },
	const struct dm9000_cfg dm9000_cfg[1] = {
#include <mach/config-dm9000.h>
	};

	dm9000_probe(&dm9000_dev, dm9000_cfg);
}
device_initcall(dm9000_init);
