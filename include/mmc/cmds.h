#ifndef	_MMC_CMDS_H_
#define	_MMC_CMDS_H_


#define	MMC_CMD_IDX_SHIFT 0
#define	MMC_CMD_ACMD		(1 << 8)
#define	MMC_CMD_RDATA		(1 << 9)
#define	MMC_CMD_WDATA		(2 << 9)
#define	MMC_CMD_DATA		(MMC_CMD_RDATA|MMC_CMD_WDATA)

#define	MMC_RSP_TYPE_SHIFT 24
#define	MMC_RSP_TYPE(X)		((X) << MMC_RSP_TYPE_SHIFT)
#define	MMC_RSP_PRESENT		(1 << 27)
#define	MMC_RSP_CRC		(1 << 28)
#define	MMC_RSP_OPCODE		(1 << 29)
#define	MMC_RSP_BUSY		(1 << 30)
#define	MMC_RSP_136		(1 << 31)

#define	MMC_RSP(TYPE, FLAGS)	(MMC_RSP_TYPE(TYPE) | FLAGS)
#define	MMC_RSP_R0	MMC_RSP(0, 0)
#define	MMC_RSP_R1	MMC_RSP(1, MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define	MMC_RSP_R1b	MMC_RSP(1, MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE|MMC_RSP_BUSY)
#define	MMC_RSP_R2	MMC_RSP(2, MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_136)
#define	MMC_RSP_R3	MMC_RSP(3, MMC_RSP_PRESENT)
#define	MMC_RSP_R4	MMC_RSP(4, MMC_RSP_PRESENT)
#define	MMC_RSP_R5	MMC_RSP(5, MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define	MMC_RSP_R6	MMC_RSP(6, MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define	MMC_RSP_R7	MMC_RSP(7, MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)


#define	MMC_CMD(IDX, RTYPE)	(IDX | RTYPE)
#define	MMC_ACMD(IDX, RTYPE)	(IDX | RTYPE | MMC_CMD_ACMD)
#define	MMC_CMD_IDX(CMD)	((CMD) & 0x3f)
#define	MMC_CMD_RTYPE(CMD)	(((CMD) >> MMC_RSP_TYPE_SHIFT) & 0x7)



#define	MMC_CMD_GO_IDLE		MMC_CMD( 0, MMC_RSP_R0)
#define	MMC_CMD_APP_CMD		MMC_CMD(55, MMC_RSP_R1)
#define	MMC_CMD_ALL_SEND_CID	MMC_CMD( 2, MMC_RSP_R2)
#define	MMC_CMD_SEND_RCA	MMC_CMD( 3, MMC_RSP_R6)
#define	MMC_CMD_SELECT_CARD	MMC_CMD( 7, MMC_RSP_R1b)
#define	MMC_CMD_SEND_CSD	MMC_CMD( 9, MMC_RSP_R2)
#define	MMC_CMD_SET_BLOCKLEN	MMC_CMD(16, MMC_RSP_R1)
#define	MMC_CMD_READ_BLOCK	MMC_CMD(17, MMC_RSP_R1|MMC_CMD_RDATA)
#define	MMC_CMD_WRITE_BLOCK	MMC_CMD(24, MMC_RSP_R1|MMC_CMD_WDATA)

#define	SDC_CMD_SEND_IF_COND	MMC_CMD( 8, MMC_RSP_R7)
#define	SDC_CMD_SET_BUS_WIDTH	MMC_ACMD( 6, MMC_RSP_R1)
#define	SDC_CMD_SEND_OP_COND	MMC_ACMD(41, MMC_RSP_R3)
#define	SDC_CMD_SEND_SCR	MMC_ACMD(51, MMC_RSP_R1|MMC_CMD_RDATA)


#endif
