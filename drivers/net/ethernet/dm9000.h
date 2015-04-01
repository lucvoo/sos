#ifndef	_DM9000_H_
#define	_DM9000_H_


#define	DM9000_ID	0x90000A46

#define	DM9000_PKT_MAX	1536	// Received packet max size

// From RX header in SRAM
#define	DM9000_PKT_RDY	0x01	// Packet ready
#define	DM9000_PKT_ERR	0x02	// Error, need reset


// Registers (16-bit but 32-bit spaced)
#define	DM9000_NCR	0x00	// Network Control Register
#define	NCR_EXT_PHY	(1<<7)
#define	NCR_WAKEEN	(1<<6)
#define	NCR_FCOL	(1<<4)
#define	NCR_FDX		(1<<3)
#define	NCR_LBK_MSK	(3<<1)
#define	NCR_LBK_NORMAL	(0<<1)	// normal
#define	NCR_LBK_MAC	(1<<1)	// MAC internal
#define	NCR_LBK_INT_PHY	(2<<1)	// internal PHY
#define	NCR_RST		(1<<0)	// Reset

#define	DM9000_NSR	0x01	// Network Status Register
#define	NSR_SPEED	(1<<7)
#define	NSR_LINKST	(1<<6)
#define	NSR_WAKEST	(1<<5)
#define	NSR_TX2END	(1<<3)
#define	NSR_TX1END	(1<<2)
#define	NSR_RXOV	(1<<1)

#define	DM9000_TCR	0x02	// Tx Control Register
#define	TCR_TJDIS	(1<<6)
#define	TCR_EXCECM	(1<<5)
#define	TCR_PAD_DIS2	(1<<4)
#define	TCR_CRC_DIS2	(1<<3)
#define	TCR_PAD_DIS1	(1<<2)
#define	TCR_CRC_DIS1	(1<<1)
#define	TCR_TXREQ	(1<<0)

#define	DM9000_TSR1	0x03	// Tx Status Registers
#define	DM9000_TSR2	0x04
#define	TSR_TJTO	(1<<7)
#define	TSR_LC		(1<<6)
#define	TSR_NC		(1<<5)
#define	TSR_LCOL	(1<<4)
#define	TSR_COL		(1<<3)
#define	TSR_EC		(1<<2)

#define	DM9000_RCR	0x05	// Rx Control Register
#define	RCR_WTDIS	(1<<6)	// Watchdog Timer Disable
#define	RCR_DIS_LONG	(1<<5)	// Discard packets too Long
#define	RCR_DIS_CRC	(1<<4)	// Discard packets with wrong CRC
#define	RCR_ALL		(1<<3)	// Pass All multicast packets
#define	RCR_RUNT	(1<<2)	// Pass Runt packets
#define	RCR_PRMSC	(1<<1)	// Promiscuous Mode
#define	RCR_RXEN	(1<<0)	// RX Enable

#define	DM9000_RSR	0x06	// Rx Status Register
#define	RSR_RF		(1<<7)	// "Runt" frame (len < 64)
#define	RSR_MF		(1<<6)	// Multicast frame
#define	RSR_LCS		(1<<5)	// Late Collision Seen
#define	RSR_RWTO	(1<<4)	// Rx Watchdof Time-Out
#define	RSR_PLE		(1<<3)	// Physical Layer Error
#define	RSR_AE		(1<<2)	// Alignment Error
#define	RSR_CE		(1<<1)	// CRC Error
#define	RSR_FOE		(1<<0)	// Fifo Overflow Error
#define	RSR_ERROR	(RSR_FOE|RSR_CE|RSR_AE|RSR_PLE|RSR_RWTO|RSR_LCS|RSR_RF)

#define	DM9000_ROCR	0x07	// Receive Overflow Counter Register

#define	DM9000_BPTR	0x08	// Back Pressure Threshold Register
#define	BPTR_BPHW(K)	(K<<4)	// Back Pressure High Water Overflow Threshold
#define	BPTR_JPT_200US	(0x07)	// Jam Pattern Time
#define	BPTR_JPT_600US	(0x0f)	// Jam Pattern Time

#define	DM9000_FCTR	0x09	// Flow Control Threshold Register
#define	FCTR_HWOT(OT)	(((OT) & 0xf) << 4)
#define	FCTR_LWOT(OT)	(((OT) & 0xf) << 0)

#define	DM9000_FCR	0x0A	// Flow Control Register
#define	FCR_TXP0	(1<<7)	// TX Pause packet time 0x0000
#define	FCR_TXPF	(1<<6)	// TX Pause packet time 0x0000
#define	FCR_TXPEN	(1<<5)	// force TX Pause Enable
#define	FCR_BKPA	(1<<4)	// Back Pressure Mode
#define	FCR_BKPM	(1<<3)	// Back Pressure Mode
#define	FCR_RXPS	(1<<2)	// RX Pause Packet Status (latch & clear ready)
#define	FCR_RXPCS	(1<<1)	// RX Pause Packet Current Status
#define	FCR_FLCE	(1<<0)	// Flow Control Enable

#define	DM9000_EPCR	0x0B	// Eeprom & Phy Control Register
#define	EPCR_REEP	(1<<5)	// Reload EEPROM
#define	EPCR_WEP	(1<<4)	// Write EEPROM
#define	EPCR_EPOS	(1<<3)	// Phy Operation Select
#define	EPCR_ERPRR	(1<<2)	// Eeprom Read / Phy Register Read
#define	EPCR_ERPRW	(1<<1)	// Eeprom Write/ Phy Register Write
#define	EPCR_ERRE	(1<<0)	// Eeprom / Phy access status

#define	DM9000_EPAR	0x0C	// Eeprom & Phy Address Register
#define	EPAR_PADR(X)	((X)<<6)// Phy Address (bit 0:1, bit 4:2 is always 0)
#define	EPAR_PREG(X)	((X)<<0)// Phy Register number
#define	EPAR_EADR(X)	((X)<<0)// Eeprom address

#define	DM9000_EPDRL	0x0D	// Eeprom & Phy Data Low  byte Register
#define	DM9000_EPDRH	0x0E	// Eeprom & Phy Data High byte Register

#define	DM9000_WCR	0x0F	// Wake up Control Register
#define	WCR_LINKEN	(1<<5)
#define	WCR_SAMPLEEN	(1<<4)
#define	WCR_MAGICEN	(1<<3)
#define	WCR_LINKST	(1<<2)
#define	WCR_SAMPLEST	(1<<1)
#define	WCR_MAGICST	(1<<0)

#define	DM9000_PAR	0x10	// Physical Address Register (0x10-0x15)
#define	DM9000_MAR	0x16	// Multicast Address Register (0x16-0x1D)

#define	DM9000_GPCR	0x1E	// General Purpose Control Register
#define	GPCR_GEP_CNTL	(1<<0)

#define	DM9000_GPR	0x1F	// General Purpose Register
#define	GPR_PHYUP	(0<<0)	// Phy powerUp
#define	GPR_PHYDOWN	(1<<0)	// Phy PowerDown

#define	DM9000_TRPAL	0x22	// Tx Read Pointer Address
#define	DM9000_TRPAH	0x23
#define	DM9000_RWPAL	0x24	// Rx Write Pointer Address
#define	DM9000_RWPAH	0x25

#define	DM9000_VIDL	0x28	// Vendor ID
#define	DM9000_VIDH	0x29
#define	DM9000_PIDL	0x2A	// Product ID
#define	DM9000_PIDH	0x2B

#define	DM9000_CHIPR	0x2C	// CHIP revision
#define	CHIPR_A		(0x19)
#define	CHIPR_B		(0x1A)

#define	DM9000_TCR2	0x2D	// Tx Control Register
#define	DM9000_OCR	0x2E	// Operation Control Register
#define	DM9000_SMCR	0x2F	// Special Mode Control Register
#define	DM9000_ETXCSR	0x30	// Early TX Constrol/Status Register

#define	DM9000_TCCR	0x31	// Tx Checksum Control Register
#define	TCCR_IP		(1<<0)
#define	TCCR_TCP	(1<<1)
#define	TCCR_UDP	(1<<2)

#define	DM9000_RCSR	0x32	// Rx Checksum Status Register
#define	RCSR_UDP_BAD	(1<<7)
#define	RCSR_TCP_BAD	(1<<6)
#define	RCSR_IP_BAD	(1<<5)
#define	RCSR_UDP	(1<<4)
#define	RCSR_TCP	(1<<3)
#define	RCSR_IP		(1<<2)
#define	RCSR_CSUM	(1<<1)
#define	RCSR_DISCARD	(1<<0)

#define	DM9000_MPAR	0x33	// Mii Phy Address Register
#define	DM9000_LEDCR	0x34	// LED Control Register
#define	DM9000_BUSCR	0x38	// processor BUS Control Register
#define	DM9000_INTCR	0x39	// INT pin Control Register
#define	DM9000_SCCR	0x50	// System Control Control Register
#define	DM9000_RSCCR	0x51	// Resume System Control Control Register

#define	DM9000_MRCMDX	0xF0	// Memory Read prefetch Command without address increment
#define	DM9000_MRCMDX1	0xF1	// Memory Read Command without address increment
#define	DM9000_MRCMD	0xF2	// Memory Read Command with address increment
#define	DM9000_MRRL	0xF4	// Memory Read address Register Low
#define	DM9000_MRRH	0xF5	// Memory Read address Register High
#define	DM9000_MWCMDX	0xF6	// Memory Write Command without address increment
#define	DM9000_MWCMD	0xF8	// Memory Write Command with address increment
#define	DM9000_MWRL	0xFA	// Memory Write address Register Low
#define	DM9000_MWRH	0xFB	// Memory Write address Register High
#define	DM9000_TXPLL	0xFC	// TX Packet Length Low byte
#define	DM9000_TXPLH	0xFD	// TX Packet Length High byte

#define	DM9000_ISR	0xFE	// Interrupt Status Register
#define	ISR_IOMODE	(1<<7)
#define	ISR_LNKCHNG	(1<<5)
#define	ISR_UNDERRUN	(1<<4)
#define	ISR_ROOS	(1<<3)
#define	ISR_ROS		(1<<2)
#define	ISR_PTS		(1<<1)
#define	ISR_PRS		(1<<0)
#define	ISR_CLR_STATUS	(ISR_ROOS|ISR_ROS|ISR_PTS|ISR_PRS)

#define	DM9000_IMR	0xFF	// Interrupt Mask Register
#define	IMR_PAR		(1<<7)
#define	IMR_ROOM	(1<<3)
#define	IMR_ROM		(1<<2)
#define	IMR_PTM		(1<<1)
#define	IMR_PRM		(1<<0)
#define	IMR_LNKCHNG	(1<<5)
#define	IMR_UNDERRUN	(1<<4)


// Davicom MII registers.
#define	MII_DM_DSPCR	0x1b    // DSP Control Register
#define	MII_DSPCR_INIT_PARAM	0xE100	// DSP init parameter

#endif
