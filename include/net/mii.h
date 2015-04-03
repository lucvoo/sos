#ifndef _NET_MII_H_
#define	_NET_MII_H_

/*
 * Linux's mii.h
 * Copyright (C) 1996, 1999, 2001 David S. Miller (davem@redhat.com)
 */

// Basic Mode Control Register
#define	MII_BMCR		0x00	// Basic Mode Control Register
#define	MII_BMCR_SPEED1000		0x0040	// MSB of Speed (1000)
#define	MII_BMCR_CTST			0x0080	// enable Collision Test
#define	MII_BMCR_FULLDPLX		0x0100	// Full Duplex
#define	MII_BMCR_ANRESTART		0x0200	// Auto Negotiation Restart
#define	MII_BMCR_ISOLATE		0x0400	// electrically Isolate phy from MII
#define	MII_BMCR_PDOWN			0x0800	// Powerdown
#define	MII_BMCR_ANENABLE		0x1000	// Auto Negotiation Enable
#define	MII_BMCR_SPEED100		0x2000	// Select 100Mbps
#define	MII_BMCR_LOOPBACK		0x4000	// Loopback enable
#define	MII_BMCR_RESET			0x8000	// phy Reset

// Basic Mode Status Register
#define	MII_BMSR		0x01	// Basic Mode Status Register
#define	MII_BMSR_ERCAP			0x0001	// Extented register capabilities
#define	MII_BMSR_JCD			0x0002	// Jabber detected
#define	MII_BMSR_LSTATUS		0x0004	// Link is up
#define	MII_BMSR_ANEGCAPABLE		0x0008	// Auto-Negotiation Capable
#define	MII_BMSR_RFAULT			0x0010	// Remote Fault detected
#define	MII_BMSR_ANEGCOMPLETE		0x0020	// Auto-negotiation Complete
#define	MII_BMSR_ESTATEN		0x0100	// Extended Status in R15
#define	MII_BMSR_100HALF2		0x0200	// 100BASE-T2 HDX capable
#define	MII_BMSR_100FULL2		0x0400	// 100BASE-T2 FDX capable
#define	MII_BMSR_10HALF			0x0800	// 10mbps, Half-duplex capable
#define	MII_BMSR_10FULL			0x1000	// 10mbps, Full-duplex capable
#define	MII_BMSR_100HALF		0x2000	// 100mbps, Half-duplex capable
#define	MII_BMSR_100FULL		0x4000	// 100mbps, Full-duplex capable
#define	MII_BMSR_100BASE4		0x8000	// 100mbps, 4k packets capable

#define	MII_PHYSID1		0x02	// PHYS ID 1
#define	MII_PHYSID2		0x03	// PHYS ID 2

// Advertisement control register
#define	MII_ADVERTISE		0x04	// Advertisement control
#define	MII_ADV_SLCT			0x001f	// Selector bits
#define	MII_ADV_CSMA			0x0001	// ...
#define	MII_ADV_10HALF			0x0020	// 10mbps Half-duplex
#define	MII_ADV_10FULL			0x0040	// 10mbps Full-duplex
#define	MII_ADV_100HALF			0x0080	// 100mbps Half-duplex
#define	MII_ADV_100FULL			0x0100	// 100mbps Full-duplex
#define	MII_ADV_1000XFULL		0x0020	// 1000BASE-X Full-duplex
#define	MII_ADV_1000XHALF		0x0040	// 1000BASE-X Half-duplex
#define	MII_ADV_1000XPAUSE		0x0080	// 1000BASE-X Pause
#define	MII_ADV_1000XPSE_ASYM		0x0100	// 1000BASE-X Pause Asym.
#define	MII_ADV_100BASE4		0x0200	// 100mbps 4k packets
#define	MII_ADV_PAUSE			0x0400	// Pause
#define	MII_ADV_PAUSE_ASYM		0x0800	// Pause Asym.
#define	MII_ADV_RFAULT			0x2000	// Fault detect
#define	MII_ADV_LPACK			0x4000	// Ack link partners response
#define	MII_ADV_NPAGE			0x8000	// Next page bit
#define	MII_ADV_HALF			(MII_ADV_10HALF|MII_ADV_100HALF)
#define	MII_ADV_FULL			(MII_ADV_10FULL|MII_ADV_100FULL|MII_ADV_CSMA)
#define	MII_ADV_ALL			(MII_ADV_10FULL|MII_ADV_100FULL|MII_ADV_HALF)

// Link Partner Ability register.
#define	MII_LPA			0x05	// Link Partner Ability
#define	MII_LPA_SLCT			MII_ADV_SLCT
#define	MII_LPA_10HALF			MII_ADV_10HALF
#define	MII_LPA_10FULL			MII_ADV_10FULL
#define	MII_LPA_100HALF			MII_ADV_100HALF
#define	MII_LPA_100FULL			MII_ADV_100FULL
#define	MII_LPA_1000XFULL		MII_ADV_1000XFULL
#define	MII_LPA_1000XHALF		MII_ADV_1000XHALF
#define	MII_LPA_1000XPAUSE		MII_ADV_1000XPAUSE
#define	MII_LPA_1000XPAUSE_ASYM		MII_ADV_1000XPAUSE_ASYM
#define	MII_LPA_100BASE4		MII_ADV_100BASE4
#define	MII_LPA_PAUSE_CAP		MII_ADV_PAUSE_CAP
#define	MII_LPA_PAUSE_ASYM		MII_ADV_PAUSE_ASYM
#define	MII_LPA_RFAULT			MII_ADV_RFAULT
#define	MII_LPA_LPACK			MII_ADV_LPACK
#define	MII_LPA_NPAGE			MII_ADV_NPAGE
#define	MII_LPA_FULL			(MII_LPA_10FULL|MII_LPA_100FULL)
#define	MII_LPA_100			(MII_LPA_100FULL|MII_LPA_100HALF|MII_LPA_100BASE4)

// Expansion register for auto-negotiation
#define	MII_EXPANSION		0x06	// Expansion register for auto-negotiation
#define	MII_EXPANSION_NWAY		0x0001	// N-way auto-nego capable
#define	MII_EXPANSION_LCWP		0x0002	// Got new RX page code word
#define	MII_EXPANSION_ENABLENPAGE	0x0004	// Enable npage words
#define	MII_EXPANSION_NPCAPABLE		0x0008	// Link partner supports npage
#define	MII_EXPANSION_MFAULTS		0x0010	// Multiple Faults detected

// Control 1000BASE-T
#define	MII_CTRL1000		0x09	// Control 1000BASE-T
#define	MII_ADV_1000FULL		0x0200	// Advertise 1000BASE-T Full duplex
#define	MII_ADV_1000HALF		0x0100	// Advertise 1000BASE-T Half duplex
#define	MII_CTL1000_AS_MASTER		0x0800
#define	MII_CTL1000_ENABLE_MASTER	0x1000

// Status 1000BASE-T
#define	MII_STAT1000		0x0a	// Status 1000BASE-T
#define	MII_LPA_1000LOCALRXOK		0x2000	// Link partner Local Receiver status
#define	MII_LPA_1000REMRXOK		0x1000	// Link partner Remote Receiver status
#define	MII_LPA_1000FULL		0x0800	// Link partner 1000BASE-T Full duplex
#define	MII_LPA_1000HALF		0x0400	// Link partner 1000BASE-T Half duplex

// MMD Control
#define	MII_MMD_CTRL		0x0d	// MMD Control
#define	MII_MMD_CTRL_DEVAD_MASK		0x001f	// DEVAD Mask
#define	MII_MMD_CTRL_ADDR		0x0000	// Address
#define	MII_MMD_CTRL_NOINCR		0x4000	// no post increment
#define	MII_MMD_CTRL_INCR_RDWT		0x8000	// post Increment on Reads & Writes
#define	MII_MMD_CTRL_INCR_ON_WT		0xC000	// post Increment on Writes only

// MMD Data
#define	MII_MMD_DATA		0x0e	// MMD Data

// Extended Status
#define	MII_ESTATUS		0x0f	// Extended Status
#define	MII_ESTATUS_1000_TFULL		0x2000	// Can do 1000BT Full
#define	MII_ESTATUS_1000_THALF		0x1000	// Can do 1000BT Half

// // Disconnect Counter
#define	MII_DCOUNTER		0x12	// Disconnect Counter

// False carrier Counter
#define	MII_FCSCOUNTER		0x13	// False carrier Counter

// N-WAY Test register.
#define	MII_NWAYTEST		0x14	// N-WAY auto-negotiation Test
#define	MII_NWAYTEST_LOOPBACK		0x0100	// Enable Loopback for N-way

#define	MII_RERRCOUNTER		0x15	// Receive error counter

#define	MII_SREVISION		0x16	// Silicon revision

#define	MII_LBRERROR		0x18	// Lpback, rx, bypass error

#define	MII_PHYADDR		0x19	// PHY address

#define	MII_TPISTATUS		0x1b	// TPI status for 10mbps

#define	MII_NCONFIG		0x1c	// Network interface config

// Flow control flags (see IEEE 802.3-2005 table 28B-3)
#define	MII_FLOW_CTRL_TX		0x0001
#define	MII_FLOW_CTRL_RX		0x0002


static inline unsigned int mii_nway_result(unsigned int nway)
{
	unsigned int result;

	if (nway & MII_LPA_100FULL)
		result = MII_LPA_100FULL;
	else if (nway & MII_LPA_100HALF)
		result = MII_LPA_100HALF;
	else if (nway & MII_LPA_10FULL)
		result = MII_LPA_10FULL;
	else
		result = MII_LPA_10HALF;

	return result;
}

static inline int mii_full_duplex(unsigned int nway, int forced)
{
	unsigned int result;

	if (forced)
		return 1;

	result = mii_nway_result(nway);
	return (result & MII_LPA_FULL) != 0;
}

#endif
