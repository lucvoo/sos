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

	mii->advertising = 0;
	mii->dev->link = 0;

	return mii_check_media(mii);
}

int mii_check_media(struct mii *mii)
{
	unsigned int adv, lpa;
	unsigned int nway;
	int duplex;
	int link;

	link = mii_link_ok(mii);
	if (link == mii->dev->link)
		return 0;

	mii->dev->link = link;
	if (!link) {
		pr_info("%s link down\n", mii->dev->ifname);
		return 0;
	}

	// get MII advertise and LPA values
	if (mii->advertising == 0) {
		mii->advertising = mii->read(mii, mii->paddr, MII_ADVERTISE);
	}
	adv = mii->advertising;
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

	return 1;
}


int mii_get_phyid(struct mii* mii, uint paddr, u32 *phy_id)
{
	int res;
	u32 id;

	res = mii->read(mii, paddr, MII_PHYSID1);
	if (res < 0)
		return -EIO;
	id = res << 16;

	res = mii->read(mii, paddr, MII_PHYSID2);
	if (res < 0)
		return -EIO;
	id |= res << 0;

	*phy_id = id;
	return 0;
}

int mii_scan_phy(struct mii* mii, uint *paddr,  u32 *phy_id)
{
	uint i;
	int rc;

	for (i = 0; i < 32; i++) {
		u32 id;

		rc = mii_get_phyid(mii, i, &id);
		if (rc)
			continue;
		if ((id & 0x1fffffff) == 0x1fffffff)
			continue;
		if ((id & 0x1fffffff) == 0x00000000)
			continue;
		*paddr = i;
		*phy_id = id;
		return 0;
	}
	return -EIO;
}
