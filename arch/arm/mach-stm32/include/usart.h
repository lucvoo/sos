#ifndef	_MACH_USART_H_
#define	_MACH_USART_H_

/*
 * USART 1-3 & 6 are fully featured
 * U/ART 4 & 5 are 'basic':
 *	no HW flow control
 *	no synchronous
 *	no smartcard support
 */

#define	USART_SR	0x00
#define	USART_SR_PE		(1 << 0)
#define	USART_SR_FE		(1 << 1)
#define	USART_SR_NF		(1 << 2)
#define	USART_SR_ORE		(1 << 3)
#define	USART_SR_IDLE		(1 << 4)
#define	USART_SR_RXNE		(1 << 5)
#define	USART_SR_TC		(1 << 6)
#define	USART_SR_TXE		(1 << 7)
#define	USART_SR_LBD		(1 << 8)
#define	USART_SR_CTS		(1 << 9)
#define	USART_SR_ERR_MASK	(USART_SR_LBD|USART_SR_ORE|USART_SR_FE|USART_SR_PE)
#define	USART_SR_DUMMY_RX	(1 << 16)

#define	USART_DR	0x04
#define	USART_DR_MASK		0x00ff

#define	USART_BRR	0x08
#define	USART_BRR_F(X)		((X) << 0)
#define	USART_BRR_M(X)		((X) << 4)

#define	USART_CR1	0x0c
#define	USART_CR1_SBK		(1 << 0)
#define	USART_CR1_RWU		(1 << 1)
#define	USART_CR1_RE		(1 << 2)
#define	USART_CR1_TE		(1 << 3)
#define	USART_CR1_IDLEIE	(1 << 4)
#define	USART_CR1_RXNEIE	(1 << 5)
#define	USART_CR1_TCIE		(1 << 6)
#define	USART_CR1_TXEIE		(1 << 7)
#define	USART_CR1_PEIE		(1 << 8)
#define	USART_CR1_PS		(1 << 9)
#define	USART_CR1_PCE		(1 << 10)
#define	USART_CR1_WAKE		(1 << 11)
#define	USART_CR1_M		(1 << 12)
#define	USART_CR1_UE		(1 << 13)
#define	USART_CR1_OVER8		(1 << 15)
#define	USART_CR1_IE_MASK	0x01f0

#define	USART_CR2	0x10
#define	USART_CR2_ADDR_MASK	0x000f
#define	USART_CR2_LBDL		(1 << 5)
#define	USART_CR2_LBDIE		(1 << 6)
#define	USART_CR2_LBCL		(1 << 8)
#define	USART_CR2_CPHA		(1 << 9)
#define	USART_CR2_CPOL		(1 << 10)
#define	USART_CR2_CLKEN		(1 << 11)
#define	USART_CR2_STOP_MASK	(3 << 12)
#define	USART_CR2_STOP_1	(0 << 12)
#define	USART_CR2_STOP_05	(1 << 12)
#define	USART_CR2_STOP_2	(2 << 12)
#define	USART_CR2_STOP_15	(3 << 12)
#define	USART_CR2_LINEN		(1 << 14)

#define	USART_CR3	0x14
#define	USART_CR3_EIE		(1 << 0)
#define	USART_CR3_IREN		(1 << 1)
#define	USART_CR3_IRLP		(1 << 2)
#define	USART_CR3_HDSEL		(1 << 3)
#define	USART_CR3_NACK		(1 << 4)
#define	USART_CR3_SCEN		(1 << 5)
#define	USART_CR3_DMAR		(1 << 6)
#define	USART_CR3_DMAT		(1 << 7)
#define	USART_CR3_RTSE		(1 << 8)
#define	USART_CR3_CTSE		(1 << 9)
#define	USART_CR3_CTSIE		(1 << 10)
#define	USART_CR3_ONEBIT	(1 << 11)

#define	USART_GTPR	0x18
#define	USART_GTPR_PSC_MASK	0x00ff
#define	USART_GTPR_GT_MASK	0xff00

#endif
