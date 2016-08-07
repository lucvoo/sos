#ifndef	_HW_PL011_H_
#define	_HW_PL011_H_

// Access to the register can be by 8 or 32 bits
#include <utils/concat.h>
#define	rd_pl011(A)	CONCAT(ioread,PL011_BITS)(A)
#define	wr_pl011(A, V)	CONCAT(iowrite,PL011_BITS)(A, V)


#define PL011_DR	0x00	// Data
#define PL011_RSR	0x04	// [R] Receive Status
#define PL011_ECR	0x04	// [W] Error Clear
#define PL011_FR	0x18	// Flag
#define PL011_FR_RI		(1 << 8)	// Ring indicator
#define PL011_FR_TXFE		(1 << 7)	// Tx fifo Empty
#define PL011_FR_RXFF		(1 << 6)	// Rx fifo Full
#define PL011_FR_TXFF		(1 << 5)	// Tx fifo Full
#define PL011_FR_RXFE		(1 << 4)	// Rx fifo Empty
#define PL011_FR_BUSY		(1 << 3)	// Busy
#define PL011_FR_DCD		(1 << 2)	//
#define PL011_FR_DSR		(1 << 1)	//
#define PL011_FR_CTS		(1 << 0)	//

#define PL011_ILPR	0x20	// IrDA Low Power counter
#define PL011_IBRD	0x24	// Integer Baud Rate Divisor
#define PL011_FBRD	0x28	// Fractional Baud Rate Divisor
#define PL011_LCR	0x2c	// Line Control
#define PL011_CR	0x30	// Control
#define PL011_IFLS	0x34	// Interrupt FIFO Level Select
#define PL011_IMSC	0x38	// Interrupt Mask Set/Clear
#define PL011_RIS	0x3c	// Raw Interrupt Status
#define PL011_MIS	0x40	// Masked Interrupt Status
#define PL011_ICR	0x44	// Interrupt Clear
#define PL011_DMACR	0x48	// DMA Control

#endif
