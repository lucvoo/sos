#ifndef	_MII_H_
#define	_MII_H_

#include <net/mii.h>

struct netdev;

// TODO: split this into mii (bus), phy (device) & phy driver.

struct mii {
	unsigned int	paddr;
	unsigned int	advertising;

	unsigned int	full_duplex:1;

	struct netdev*	dev;
	int		(*read)(struct mii *mii, unsigned int paddr, unsigned int reg);
	int		(*write)(struct mii *mii, unsigned int paddr, unsigned int reg, unsigned int val);
};


int mii_link_ok(struct mii *mii);
int mii_nway_restart(struct mii *mii);
int mii_init_media(struct mii *mii);
int mii_check_media(struct mii *mii);

int mii_get_phyid(struct mii *mii, uint paddr, u32 *phy_id);
int mii_scan_phy(struct mii *mii, uint *paddr, u32 *phy_id);

#endif
