#include <mii.h>
#include <errno.h>
#include <net/dev.h>


int mii_link_ok(struct mii *mii)
{
	unsigned int bmsr;

	// First a dummy read to latch ...
	bmsr = mii->read(mii, mii->paddr, MII_BMSR);

	// Now the real read
	bmsr = mii->read(mii, mii->paddr, MII_BMSR);

	return (bmsr & MII_BMSR_LSTATUS) != 0;
}

int mii_nway_restart(struct mii *mii)
{
	unsigned int bmcr;
	int rc;

	bmcr = mii->read(mii, mii->paddr, MII_BMCR);
	if (!(bmcr & MII_BMCR_ANENABLE))
		return -EINVAL;

	bmcr |= MII_BMCR_ANRESTART;
	rc = mii->write(mii, mii->paddr, MII_BMCR, bmcr);

	return rc;
}

static int mii_speed(unsigned int nway)
{
	// TODO: LPA_1000

	if (nway & MII_LPA_100)
		return 100;

	return 10;
}

int mii_init_media(struct mii *mii)
{
	unsigned int adv, lpa;
	unsigned int nway;
	int duplex;
	int link;

	mii->advertising = 0;
	link = mii_link_ok(mii);
	if (!link)
		return -ENODEV;

	adv = mii->read(mii, mii->paddr, MII_ADVERTISE);
	mii->advertising = adv;

	lpa = mii->read(mii, mii->paddr, MII_LPA);

	// TODO: GMII

	// combine LPA result with what is advertised
	nway = mii_nway_result(adv & lpa);
	duplex = (nway & MII_LPA_FULL) != 0;
	mii->full_duplex = duplex;

	pr_info("%s: link up, %uMbps, %s-duplex, lpa 0x%04X\n",
			mii->dev->ifname,
			mii_speed(nway),
			duplex ? "full" : "half",
			lpa);

	return 0;
}
