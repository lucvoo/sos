#ifndef	_IMX6_ENET_H_
#define	_IMX6_ENET_H_

// Configuration
#define	ENET_EIR	0x004	// Interrupt Event
#define	     EIR_MII		(1 << 23)
#define	ENET_EIMR	0x008	// Interrupt Mask
#define	ENET_RDAR0	0x010	// Rx Descriptor Active - Ring 0
#define	ENET_TDAR0	0x014	// Tx Descriptor Active - Ring 0
#define	ENET_ECR	0x024	// Ethernet Control
#define      ECR_ETHEREN	(1 << 1)
#define      ECR_RESET		(1 << 0)

#define	ENET_MMFR	0x040	// MII Management Frame
#define	     MMFR_DATA_MASK	0xFFFF
#define	     MMFR_DATA(V)	((V) & MMFR_DATA_MASK)
#define	     MMFR_TA		(2 << 16)
#define	     MMFR_RA(A)		((A) << 18)
#define	     MMFR_PA(A)		((A) << 23)
#define	     MMFR_OP_WRITE	(1 << 28)
#define	     MMFR_OP_READ	(2 << 28)
#define	     MMFR_ST		(1 << 30)
#define	     MMFR_OP(P, R)	(MMFR_ST|MMFR_TA|MMFR_PA(P)|MMFR_RA(R))
#define	     MMFR_READ(P, R)	(MMFR_OP(P, R)|MMFR_OP_READ)
#define	     MMFR_WRITE(P,R,V)	(MMFR_OP(P, R)|MMFR_OP_WRITE|MMFR_DATA(V))

#define	ENET_MSCR	0x044	// MII Speed Control
#define	     MSCR_SPEED(S)	(((S) - 1) << 1)
#define	     MSCR_HOLD(H)	(((H) - 1) << 8)
#define	ENET_MIBC	0x064	// MIB Control
#define	ENET_RCR	0x084	// Rx Control
#define	ENET_TCR	0x0C4	// Tx Control
#define	ENET_PALR	0x0E4	// Physical Address Lower
#define	ENET_PAUR	0x0E8	// Physical Address Upper
#define	ENET_OPD	0x0EC	// Opcode/Pause Duration
#define	ENET_TXIC0	0x0F0	// Tx Interrupt Coalescing
#define	ENET_TXIC1	0x0F4	// Tx Interrupt Coalescing
#define	ENET_TXIC2	0x0F8	// Tx Interrupt Coalescing
#define	ENET_RXIC0	0x100	// Rx Interrupt Coalescing
#define	ENET_RXIC1	0x104	// Rx Interrupt Coalescing
#define	ENET_RXIC2	0x108	// Rx Interrupt Coalescing
#define	ENET_IAUR	0x118	// Descriptor Individual Upper Address
#define	ENET_IALR	0x11C	// Descriptor Individual Lower Address
#define	ENET_GAUR	0x120	// Descriptor Group Upper Address
#define	ENET_GALR	0x124	// Descriptor Group Lower Address
#define	ENET_TFWR	0x144	// Tx FIFO Watermark
#define	ENET_RDSR1	0x160	// Rx Descriptor Ring 1 Start
#define	ENET_TDSR1	0x164	// Tx Buffer Descriptor Ring 1 Start
#define	ENET_MRBR1	0x168	// Maximum Rx Buffer Size - Ring 1
#define	ENET_RDSR2	0x16C	// Rx Descriptor Ring 2 Start
#define	ENET_TDSR2	0x170	// Tx Buffer Descriptor Ring 2 Start
#define	ENET_MRBR2	0x174	// Maximum Rx Buffer Size - Ring 2
#define	ENET_RDSR0	0x180	// Rx Descriptor Ring 0 Start
#define	ENET_TDSR0	0x184	// Tx Buffer Descriptor Ring 0 Start
#define	ENET_MRBR0	0x188	// Maximum Rx Buffer Size - Ring 0
#define	ENET_RSFL	0x190	// Rx FIFO Section Full Threshold
#define	ENET_RSEM	0x194	// Rx FIFO Section Empty Threshold
#define	ENET_RAEM	0x198	// Rx FIFO Almost Empty Threshold
#define	ENET_RAFL	0x19C	// Rx FIFO Almost Full Threshold
#define	ENET_TSEM	0x1A0	// Tx FIFO Section Empty Threshold
#define	ENET_TAEM	0x1A4	// Tx FIFO Almost Empty Threshold
#define	ENET_TAFL	0x1A8	// Tx FIFO Almost Full Threshold
#define	ENET_TIPG	0x1AC	// Tx Inter-Packet Gap
#define	ENET_FTRL	0x1B0	// Frame Truncation Length
#define	ENET_TACC	0x1C0	// Tx Accelerator Function Configuration
#define	ENET_RACC	0x1C4	// Rx Accelerator Function Configuration
#define	ENET_RCMR1	0x1C8	// Rx Classification Match for Class n
#define	ENET_RCMR2	0x1CC	// Rx Classification Match for Class n
#define	ENET_DMA1CFG	0x1D8	// DMA Class Based Configuration
#define	ENET_DMA2CFG	0x1DC	// DMA Class Based Configuration
#define	ENET_RDAR1	0x1E0	// Rx Descriptor Active - Ring 1
#define	ENET_TDAR1	0x1E4	// Tx Descriptor Active - Ring 1
#define	ENET_RDAR2	0x1E8	// Rx Descriptor Active - Ring 2
#define	ENET_TDAR2	0x1EC	// Tx Descriptor Active - Ring 2
#define	ENET_QOS	0x1F0	// QOS Scheme

// Statistics counters
#define	ENET_RMON_T_DROP	0x200	// Reserved
#define	ENET_RMON_T_PACKETS	0x204	// Tx Packet Count
#define	ENET_RMON_T_BC_PKT	0x208	// Tx Broadcast Packets
#define	ENET_RMON_T_MC_PKT	0x20C	// Tx Multicast Packets
#define	ENET_RMON_T_CRC_ALIGN	0x210	// Tx Packets with CRC/Align Error
#define	ENET_RMON_T_UNDERSIZE	0x214	// Tx Packets Less Than Bytes and Good CRC
#define	ENET_RMON_T_OVERSIZE	0x218	// Tx Packets GT MAX_FL bytes and Good CRC
#define	ENET_RMON_T_FRAG	0x21C	// Tx Packets Less Than 64 Bytes and Bad CRC
#define	ENET_RMON_T_JAB		0x220	// Tx Packets Greater Than MAX_FL bytes and Bad CRC
#define	ENET_RMON_T_COL		0x224	// Tx Collision Count
#define	ENET_RMON_T_P64		0x228	// Tx 64 byte Packets
#define	ENET_RMON_T_P65TO127	0x22C	// Tx 65 to 127 byte Packets
#define	ENET_RMON_T_P128TO255	0x230	// Tx 128 to 255 byte Packets
#define	ENET_RMON_T_P256TO511	0x234	// Tx 256 to 511 byte Packets
#define	ENET_RMON_T_P512TO1023	0x238	// Tx 512 to 1023 byte Packets
#define	ENET_RMON_T_P1024TO2047	0x23C	// Tx 1024 to 2047 byte Packets
#define	ENET_RMON_T_P_GTE2048	0x240	// Tx Packets Greater Than 2048 Bytes
#define	ENET_RMON_T_OCTETS	0x244	// Tx Octets
#define	ENET_IEEE_T_DROP	0x248	// IEEE_T_DROP Reserved
#define	ENET_IEEE_T_FRAME_OK	0x24C	// Frames Transmitted OK
#define	ENET_IEEE_T_1COL	0x250	// Frames Transmitted with Single Collision
#define	ENET_IEEE_T_MCOL	0x254	// Frames Transmitted with Multiple Collisions
#define	ENET_IEEE_T_DEF		0x258	// Frames Transmitted after Deferral Delay
#define	ENET_IEEE_T_LCOL	0x25C	// Frames Transmitted with Late Collision
#define	ENET_IEEE_T_EXCOL	0x260	// Frames Transmitted with Excessive Collisions
#define	ENET_IEEE_T_MACERR	0x264	// Frames Transmitted with Tx FIFO Underrun
#define	ENET_IEEE_T_CSERR	0x268	// Frames Transmitted with Carrier Sense Error
#define	ENET_IEEE_T_SQE		0x26C	//
#define	ENET_IEEE_T_FDXFC	0x270	// Flow Control Pause Frames Transmitted
#define	ENET_IEEE_T_OCTETS_OK	0x274	// Octet Count for Frames Transmitted w/o Error
#define	ENET_RMON_R_PACKETS	0x284	// Rx Packet Count
#define	ENET_RMON_R_BC_PKT	0x288	// Rx Broadcast Packets
#define	ENET_RMON_R_MC_PKT	0x28C	// Rx Multicast Packets
#define	ENET_RMON_R_CRC_ALIGN	0x290	// Rx Packets with CRC/Align Error
#define	ENET_RMON_R_UNDERSIZE	0x294	// Rx Packets with Less Than 64 Bytes and Good CRC
#define	ENET_RMON_R_OVERSIZE	0x298	// Rx Packets Greater Than MAX_FL and Good CRC
#define	ENET_RMON_R_FRAG	0x29C	// Rx Packets Less Than 64 Bytes and Bad CRC
#define	ENET_RMON_R_JAB		0x2A0	// Rx Packets Greater Than MAX_FL Bytes and Bad CRC
#define	ENET_RMON_R_RESVD_0	0x2A4	// Reserved
#define	ENET_RMON_R_P64		0x2A8	// Rx 64 byte Packets
#define	ENET_RMON_R_P65TO127	0x2AC	// Rx 65 to 127 byte Packets
#define	ENET_RMON_R_P128TO255	0x2B0	// Rx 128 to 255 byte Packets
#define	ENET_RMON_R_P256TO511	0x2B4	// Rx 256 to 511 byte Packets
#define	ENET_RMON_R_P512TO1023	0x2B8	// Rx 512 to 1023 byte Packets
#define	ENET_RMON_R_P1024TO2047	0x2BC	// Rx 1024 to 2047 byte Packets
#define	ENET_RMON_R_P_GTE2048	0x2C0	// Rx Packets Greater than 2048 Bytes
#define	ENET_RMON_R_OCTETS	0x2C4	// Rx Octets
#define	ENET_IEEE_R_DROP	0x2C8	// Frames not Counted Correctly
#define	ENET_IEEE_R_FRAME_OK	0x2CC	// Frames Received OK
#define	ENET_IEEE_R_CRC		0x2D0	// Frames Received with CRC Error
#define	ENET_IEEE_R_ALIGN	0x2D4	// Frames Received with Alignment Error
#define	ENET_IEEE_R_MACERR	0x2D8	// Rx FIFO Overflow Count
#define	ENET_IEEE_R_FDXFC	0x2DC	// Flow Control Pause Frames Received
#define	ENET_IEEE_R_OCTETS_OK	0x2E0	// Octet Count for Frames Received without Error

// ieee 1588 control
#define	ENET_ATCR	0x400	// Adjustable Timer Control
#define	ENET_ATVR	0x404	// Timer Value
#define	ENET_ATOFF	0x408	// Timer Offset
#define	ENET_ATPER	0x40C	// Timer Period
#define	ENET_ATCOR	0x410	// Timer Correction
#define	ENET_ATINC	0x414	// Time-Stamping Clock Period
#define	ENET_ATSTMP	0x418	// Timestamp of Last Transmitted Frame

// capture/compare
#define	ENET_TGSR	0x604	// Timer Global Status
#define	ENET_TCSR0	0x608	// Timer Control Status
#define	ENET_TCCR0	0x60C	// Timer Compare Capture
#define	ENET_TCSR1	0x610	// Timer Control Status
#define	ENET_TCCR1	0x614	// Timer Compare Capture
#define	ENET_TCSR2	0x618	// Timer Control Status
#define	ENET_TCCR2	0x61C	// Timer Compare Capture
#define	ENET_TCSR3	0x620	// Timer Control Status
#define	ENET_TCCR3	0x624	// Timer Compare Capture

#endif
