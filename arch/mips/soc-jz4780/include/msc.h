#ifndef _SOC_JZ4780_MSC_H_
#define _SOC_JZ4780_MSC_H_

/*
 * Ingenic JZ4780 MMC/SD Controller driver header file
 *
 * Copyright (C) 2012 Ingenic Semiconductor Co., Ltd.
 * Written by Large Dipper <ykli@ingenic.com>.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */


#define	MSC0_BASE	0x13450000
#define	MSC1_BASE	0x13460000
#define	MSC2_BASE	0x13470000


// MSC Clock and Control Register	(WO, 16 bit)
#define	MSC_CTRL		0x000
#define CTRL_SEND_CCSD			(1 << 15) // send command completion signal disable to CE-ATA
#define CTRL_SEND_AS_CCSD		(1 << 14) // send auto stop after sending CCSD
#define CTRL_EXIT_MULTIPLE		(1 << 7)
#define CTRL_EXIT_TRANSFER		(1 << 6)
#define CTRL_START_READWAIT		(1 << 5)
#define CTRL_STOP_READWAIT		(1 << 4)
#define CTRL_RESET			(1 << 3)
#define CTRL_START_OP			(1 << 2)
#define CTRL_CLOCK_SHIFT		0
#define CTRL_CLOCK_MASK			(3 << CTRL_CLOCK_SHIFT)
#define CTRL_CLOCK_STOP			(1 << CTRL_CLOCK_SHIFT) // Stop MMC/SD clock
#define CTRL_CLOCK_START		(2 << CTRL_CLOCK_SHIFT) // Start MMC/SD clock

// MSC Control 2 Register		(RW, 32)
#define	MSC_CTRL2		0x058
#define	CTRL2_PIP_SHIFT			24
#define	CTRL2_PIP_MASK			(0x1f << CTRL2_PIP_SHIFT)
#define	CTRL2_RST_EN			(1 << 23)
#define	CTRL2_STPRM			(1 << 4)
#define	CTRL2_SVC			(1 << 3)
#define	CTRL2_SMS_SHIFT			0
#define	CTRL2_SMS_MASK			(7 << CTRL2_SMS_SHIFT)
#define	CTRL2_SMS_DEFSPD		(0 << CTRL2_SMS_SHIFT)
#define	CTRL2_SMS_HISPD			(1 << CTRL2_SMS_SHIFT)
#define	CTRL2_SMS_SDR12			(2 << CTRL2_SMS_SHIFT)
#define	CTRL2_SMS_SDR25			(3 << CTRL2_SMS_SHIFT)
#define	CTRL2_SMS_SDR50			(4 << CTRL2_SMS_SHIFT)

// MSC Status Register			(RO, 32bit)
#define	MSC_STAT		0x004
#define STAT_AUTO_CMD12_DONE		(1 << 31)
#define STAT_AUTO_CMD23_DONE		(1 << 30)
#define STAT_SVS			(1 << 29)
#define STAT_PIN_LEVEL_SHIFT		24
#define STAT_PIN_LEVEL_MASK		(0x1f << STAT_PIN_LEVEL_SHIFT)
#define STAT_BCE			(1 << 20)
#define STAT_BDE			(1 << 19)
#define STAT_BAE			(1 << 18)
#define STAT_BAR			(1 << 17)
#define STAT_IS_RESETTING		(1 << 15)
#define STAT_SDIO_INT_ACTIVE		(1 << 14)
#define STAT_PRG_DONE			(1 << 13)
#define STAT_DATA_TRAN_DONE		(1 << 12)
#define STAT_END_CMD_RES		(1 << 11)
#define STAT_DATA_FIFO_AFULL		(1 << 10)
#define STAT_IS_READWAIT		(1 << 9)
#define STAT_CLK_EN			(1 << 8)
#define STAT_DATA_FIFO_FULL		(1 << 7)
#define STAT_DATA_FIFO_EMPTY		(1 << 6)
#define STAT_CRC_RES_ERR		(1 << 5)
#define STAT_CRC_READ_ERROR		(1 << 4)
#define STAT_CRC_WRITE_ERROR_SHIFT	2
#define STAT_CRC_WRITE_ERROR_MASK	(3 << STAT_CRC_WRITE_ERROR_SHIFT)
#define STAT_CRC_WRITE_ERROR_NO		(0 << STAT_CRC_WRITE_ERROR_SHIFT)
#define STAT_CRC_WRITE_ERROR		(1 << STAT_CRC_WRITE_ERROR_SHIFT)
#define STAT_CRC_WRITE_ERROR_NOSTS	(2 << STAT_CRC_WRITE_ERROR_SHIFT)
#define STAT_TIME_OUT_RES		(1 << 1)
#define STAT_TIME_OUT_READ		(1 << 0)
#define	STAT_ERR			(0x3f)

// MSC Bus Clock Control Register	(RW, 16bit)
#define	MSC_CLKRT		0x008
#define	CLKRT_CLK_RATE_SHIFT		0
#define	CLKRT_CLK_RATE_MASK		(7 << CLKRT_CLK_RATE_SHIFT)
#define CLKRT_CLK_RATE_DIV_1		(0 << CLKRT_CLK_RATE_SHIFT) // CLK_SRC
#define CLKRT_CLK_RATE_DIV_2		(1 << CLKRT_CLK_RATE_SHIFT) // 1/2 of CLK_SRC
#define CLKRT_CLK_RATE_DIV_4		(2 << CLKRT_CLK_RATE_SHIFT) // 1/4 of CLK_SRC
#define CLKRT_CLK_RATE_DIV_8		(3 << CLKRT_CLK_RATE_SHIFT) // 1/8 of CLK_SRC
#define CLKRT_CLK_RATE_DIV_16		(4 << CLKRT_CLK_RATE_SHIFT) // 1/16 of CLK_SRC
#define CLKRT_CLK_RATE_DIV_32		(5 << CLKRT_CLK_RATE_SHIFT) // 1/32 of CLK_SRC
#define CLKRT_CLK_RATE_DIV_64		(6 << CLKRT_CLK_RATE_SHIFT) // 1/64 of CLK_SRC
#define CLKRT_CLK_RATE_DIV_128		(7 << CLKRT_CLK_RATE_SHIFT) // 1/128 of CLK_SRC

// MSC Command and Data Control Reg.	(RW, 32)
#define	MSC_CMDAT		0x00C
#define	CMDAT_CCS_EXPECTED		(1 << 31) // interrupts are enabled in ce-ata
#define	CMDAT_READ_CEATA		(1 << 30)
#define	CMDAT_DIS_BOOT			(1 << 27)
#define	CMDAT_ENB_BOOT			(1 << 26)
#define	CMDAT_EXP_BOOT_ACK		(1 << 25)
#define	CMDAT_BOOT_MODE			(1 << 24)
#define	CMDAT_AUTO_CMD23		(1 << 18)
#define	CMDAT_SDIO_PRDT			(1 << 17) // exact 2 cycle
#define	CMDAT_AUTO_CMD12		(1 << 16)
#define	CMDAT_RTRG_SHIFT		14
#define CMDAT_RTRG_EQUALT_16	        (0 << CMDAT_RTRG_SHIFT) // reset value
#define CMDAT_RTRG_EQUALT_32		(1 << CMDAT_RTRG_SHIFT)
#define CMDAT_RTRG_EQUALT_64		(2 << CMDAT_RTRG_SHIFT)
#define CMDAT_RTRG_EQUALT_96		(3 << CMDAT_RTRG_SHIFT)
#define	CMDAT_TTRG_SHIFT		12
#define CMDAT_TTRG_LESS_16		(0 << CMDAT_TTRG_SHIFT) // reset value
#define CMDAT_TTRG_LESS_32		(1 << CMDAT_TTRG_SHIFT)
#define CMDAT_TTRG_LESS_64		(2 << CMDAT_TTRG_SHIFT)
#define CMDAT_TTRG_LESS_96		(3 << CMDAT_TTRG_SHIFT)
#define	CMDAT_IO_ABORT			(1 << 11)
#define	CMDAT_BUS_WIDTH_SHIFT		9
#define	CMDAT_BUS_WIDTH_MASK		(3 << CMDAT_BUS_WIDTH_SHIFT)
#define CMDAT_BUS_WIDTH_1BIT		(0 << CMDAT_BUS_WIDTH_SHIFT) // 1-bit data bus
#define CMDAT_BUS_WIDTH_4BIT		(2 << CMDAT_BUS_WIDTH_SHIFT) // 4-bit data bus
#define CMDAT_BUS_WIDTH_8BIT		(3 << CMDAT_BUS_WIDTH_SHIFT) // 8-bit data bus
#define	CMDAT_INIT			(1 << 7)
#define	CMDAT_BUSY			(1 << 6)
#define	CMDAT_STREAM			(1 << 5)
#define	CMDAT_WRITE			(1 << 4)
#define	CMDAT_DATA_EN			(1 << 3)
#define	CMDAT_RESPONSE_SHIFT		0
#define	CMDAT_RESPONSE_MASK		(7 << CMDAT_RESPONSE_SHIFT)
#define CMDAT_RESPONSE_NONE		(0 << CMDAT_RESPONSE_SHIFT) // No response
#define CMDAT_RESPONSE_R1		(1 << CMDAT_RESPONSE_SHIFT) // Format R1 and R1b
#define CMDAT_RESPONSE_R2		(2 << CMDAT_RESPONSE_SHIFT) // Format R2
#define CMDAT_RESPONSE_R3		(3 << CMDAT_RESPONSE_SHIFT) // Format R3
#define CMDAT_RESPONSE_R4		(4 << CMDAT_RESPONSE_SHIFT) // Format R4
#define CMDAT_RESPONSE_R5		(5 << CMDAT_RESPONSE_SHIFT) // Format R5
#define CMDAT_RESPONSE_R6		(6 << CMDAT_RESPONSE_SHIFT) // Format R6
#define CMDAT_RESPONSE_R7		(7 << CMDAT_RESPONSE_SHIFT) // Format R7

// MSC Response Time Out Register	(RW, 16bit)
#define	MSC_RESTO		0x010

// MSC Read Time Out Register		(RW, 32bit)
#define	MSC_RDTO		0x014

// MSC Block Size Register		(RW, 16bit)
#define	MSC_BLKLEN		0x018

// MSC Block Count Register		(RW, 16bit)
#define	MSC_NOB			0x01C

// MSC Success Blocks Count Register	(RW, 16bit)
#define	MSC_SNOB		0x020

// MSC Interrupts Mask Register		(RW, 32bit)
// MSC Interrupts Flags Register	(RW, 32bit)
#define	MSC_IMASK		0x024
#define	MSC_IFLG		0x028
#define	INT_AUTO_CMD23_DONE		(1 << 30)
#define	INT_SVS				(1 << 29)
#define	INT_PIN_LEVEL_SHIFT		24
#define	INT_PIN_LEVEL_MASK		(0x1f << IMASK_PIN_LEVEL_SHIFT)
#define	INT_BCE				(1 << 20)
#define	INT_BDE				(1 << 19)
#define	INT_BAE				(1 << 18)
#define	INT_BAR				(1 << 17)
#define	INT_DMAEND			(1 << 16)
#define	INT_AUTO_CMD12_DONE		(1 << 15)
#define	INT_DATA_FIFO_FULL		(1 << 14)
#define	INT_DATA_FIFO_EMP		(1 << 13)
#define	INT_CRC_RES_ERR			(1 << 12)
#define	INT_CRC_READ_ERR		(1 << 11)
#define	INT_CRC_WRITE_ERR		(1 << 10)
#define	INT_TIME_OUT_RES		(1 << 9)
#define	INT_TIME_OUT_READ		(1 << 8)
#define	INT_SDIO			(1 << 7)
#define	INT_TXFIFO_WR_REQ		(1 << 6)
#define	INT_RXFIFO_RD_REQ		(1 << 5)
#define	INT_END_CMD_RES			(1 << 2)
#define	INT_PRG_DONE			(1 << 1)
#define	INT_DATA_TRAN_DONE		(1 << 0)

// MSC Command Index Register		(RW, 8bit)
#define	MSC_CMD			0x02C

// MSC Command Argument Register	(RW, 32bit)
#define	MSC_ARG			0x030

//MSC Response FIFO Register		(RO, 16bit)
#define	MSC_RES			0x034

// MSC Receive FIFO Register		(RO, 32bit)
#define	MSC_RXFIFO		0x038

// MSC Transmit FIFO Register		(WO, 32bit)
#define	MSC_TXFIFO		0x03C

// MSC Low Power Mode Register		(RW, 32bit)
#define	MSC_LPM			0x040
#define	LPM_DRV_SEL_SHIFT		30
#define	LPM_DRV_SEL_MASK		(3 << LPM_DRV_SEL_SHIFT)
#define	LPM_SMP_SEL			(1 << 29)
#define	LPM_LPM				(1 << 0)

// MSC DMA Control Register		(RW, 32bit)
#define	MSC_DMAC		0x044
#define	DMAC_MODE_SEL			(1 << 7)
#define	DMAC_AOFST_SHIFT		5
#define	DMAC_AOFST_MASK			(3 << DMAC_AOFST_SHIFT)
#define	DMAC_AOFST_0			(0 << DMAC_AOFST_SHIFT)
#define	DMAC_AOFST_1			(1 << DMAC_AOFST_SHIFT)
#define	DMAC_AOFST_2			(2 << DMAC_AOFST_SHIFT)
#define	DMAC_AOFST_3			(3 << DMAC_AOFST_SHIFT)
#define	DMAC_ALIGNEN			(1 << 4)
#define	DMAC_INCR_SHIFT			2
#define	DMAC_INCR_MASK			(3 << DMAC_INCR_SHIFT)
#define	DMAC_INCR_16			(0 << DMAC_INCR_SHIFT)
#define	DMAC_INCR_32			(1 << DMAC_INCR_SHIFT)
#define	DMAC_INCR_64			(2 << DMAC_INCR_SHIFT)
#define	DMAC_DMASEL			(1 << 1)
#define	DMAC_DMAEN			(1 << 0)

// MSC DMA Descriptor Address Register	(RW, 32bit)
#define	MSC_DMANDA		0x048

// MSC DMA Data Address Register	(RW, 32bit)
#define	MSC_DMADA		0x04C

// MSC DMA Length Register		(RW, 32bit)
#define	MSC_DMALEN		0x050

// MSC DMA Command Register		(RW, 32bit)
#define	MSC_DMACMD		0x054
#define	DMACMD_IDI_SHIFT		24
#define	DMACMD_IDI_MASK			(0xff << DMACMD_IDI_SHIFT)
#define	DMACMD_ID_SHIFT			16
#define	DMACMD_ID_MASK			(0xff << DMACMD_ID_SHIFT)
#define	DMACMD_OFFSET_SHIFT		9
#define	DMACMD_OFFSET_MASK		(3 << DMACMD_OFFSET_SHIFT)
#define	DMACMD_ALIGN_EN			(1 << 8)
#define	DMACMD_ENDI			(1 << 1)
#define	DMACMD_LINK			(1 << 0)

// MSC RTFIFO Data Counter Register	(RO, 32bit)
#define	MSC_RTCNT		0x05C


#define MMC_BOOT_AREA_PROTECTED		0x1234	// Can not modified the area protected
#define MMC_BOOT_AREA_OPENED		0x4321	// Can modified the area protected

#endif
