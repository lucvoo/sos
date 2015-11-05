#ifndef	_SOC_SERIAL_H_
#define	_SOC_SERIAL_H_

#define	UART1_BASE	0x02020000
#define	UART2_BASE	0x021E8000
#define	UART3_BASE	0x021EC000
#define	UART4_BASE	0x021F0000
#define	UART5_BASE	0x021F4000


#define	UART_URXD	0x00	// Receiver Register
#define	     URXD_CHARRDY	(1<<15)
#define	     URXD_ERR		(1<<14)
#define	     URXD_OVRRUN	(1<<13)
#define	     URXD_FRMERR	(1<<12)
#define	     URXD_BRK		(1<<11)
#define	     URXD_PRERR		(1<<10)
#define	     URXD_DATA		(0xFF)
#define	UART_UTXD	0x40	// Transmitter Register
#define	UART_UCR1	0x80	// Control Register 1
#define	     UCR1_ADEN		(1<<15)	// Auto Detect Interrupt
#define	     UCR1_ADBR		(1<<14)	// Auto Detect Baud Rate
#define	     UCR1_TRDYEN	(1<<13)	// Transmitter Ready Interrupt Enable
#define	     UCR1_IDEN		(1<<12)	// Idle Condition Interrupt
#define	     UCR1_ICD_4		(0<<10)	// Idle Condition Detect
#define	     UCR1_ICD_8		(1<<10)
#define	     UCR1_ICD_16	(2<<10)
#define	     UCR1_ICD_32	(3<<10)
#define	     UCR1_RRDYEN	(1<< 9)	// Recv Ready Interrupt Enable
#define	     UCR1_RDMAEN	(1<< 8)	// Recv Ready DMA Enable
#define	     UCR1_IREN		(1<< 7)	// Infrared Interface Enable
#define	     UCR1_TXMPTYEN	(1<< 6)	// Transimitter Empty Interrupt Enable
#define	     UCR1_RTSDEN	(1<< 5)	// RTS Delta Interrupt Enable
#define	     UCR1_SNDBRK	(1<< 4)	// Send Break
#define	     UCR1_TDMAEN	(1<< 3)	// Transmitter Ready DMA Enable
#define	     UCR1_ATDMAEN	(1<< 2)	// Aging DMA Timer Enable
#define	     UCR1_DOZE		(1<< 1)	// Doze
#define	     UCR1_UARTEN	(1<< 0)	// UART Enabled
#define	UART_UCR2	0x84	// Control Register 2
#define	     UCR2_ESCI		(1<<15)	// Escape Sequence Interrupt Enable
#define	     UCR2_IRTS		(1<<14)	// Ignore RTS Pin
#define	     UCR2_CTSC		(1<<13)	// CTS Pin Control
#define	     UCR2_CTS		(1<<12)	// Clear to Send
#define	     UCR2_ESCEN		(1<<11)	// Escape Enable
#define	     UCR2_RTEC_RISING	(0<< 9)	// RTS Edge Control
#define	     UCR2_RTEC_FAILING	(1<< 9)
#define	     UCR2_RTEC_ANY	(2<< 9)
#define	     UCR2_PREN		(1<< 8)	// Parity Enable
#define	     UCR2_PROE		(1<< 7)	// Parity Odd/Even
#define	     UCR2_STPB		(1<< 6)	// Stop
#define	     UCR2_WS		(1<< 5)	// Word Size
#define	     UCR2_RTSEN		(1<< 4)	// Request to Send Interrupt Enable
#define	     UCR2_ATEN		(1<< 3)	// Aging Timer Enable
#define	     UCR2_TXEN		(1<< 2)	// Transmitter Enabled
#define	     UCR2_RXEN		(1<< 1)	// Receiver Enabled
#define	     UCR2_SRST		(1<< 0)	// SW Reset
#define	UART_UCR3	0x88	// Control Register 3
#define	     UCR3_DPEC_RISING	(0<< 9)	// DTR/DSR Interrupt Edge Control
#define	     UCR3_DPEC_FAILING	(1<< 9)
#define	     UCR3_DPEC_ANY	(2<< 9)
#define	     UCR3_DTREN		(1<<13)	// DTR Interrupt Enable
#define	     UCR3_PARERREN	(1<<12)	// Parity Error Interrupt Enable
#define	     UCR3_FRAERREN	(1<<11)	// Frame Error Interrupt Enable
#define	     UCR3_DSR		(1<<10)	// Data Set Ready
#define	     UCR3_DCD		(1<< 9)	// Data Carrier Detect
#define	     UCR3_RI		(1<< 8)	// Ring Indicator
#define	     UCR3_ADNIMP	(1<< 7)	// Autobaud Detection Not Improved
#define	     UCR3_RXDSEN	(1<< 6)	// Receive Status Interrupt Enable
#define	     UCR3_AIRINTEN	(1<< 5)	// Async IR Wake Interrupt Enable
#define	     UCR3_AWAKEN	(1<< 4)	// Async Wake Interrupt Enable
#define	     UCR3_DTRDEN	(1<< 3)	// DTR Delta Enable
#define	     UCR3_RXDMUXSEL	(1<< 2)	// RXD Muxed Input Select
#define	     UCR3_INVT		(1<< 1)	// Inverted Infrared Transmission
#define	     UCR3_ACIEN		(1<< 0)	// Autobaud Counter Interrupt Enable
#define	UART_UCR4	0x8C	// Control Register 4
#define	     UCR4_CTSTL(X)	((X) <<10)	// CTS Trigger Level
#define	     UCR4_INVR		(1<< 9)	// Inverted Infrared Reception
#define	     UCR4_ENIRI		(1<< 8)	// Serial Infrared Interrupt Enable
#define	     UCR4_WKEN		(1<< 7)	// Wake Interrupt Enable
#define	     UCR4_IDDMAEN	(1<< 6)	// DMA IDLE Condition Detected
#define	     UCR4_IRSC		(1<< 5)	// IR Special Case
#define	     UCR4_LPBYP		(1<< 3)	// Low Power Bypass
#define	     UCR4_TCEN		(1<< 3)	// Transmit Complete Interrupt Enable
#define	     UCR4_BKEN		(1<< 2)	// Break Condition Interrupt Enable
#define	     UCR4_OREN		(1<< 1)	// Receiver Overrun Interrupt Enable
#define	     UCR4_DREN		(1<< 0)	// Receive Data Ready Interrupt Enable
#define	UART_UFCR	0x90	// FIFO Control Register
#define	     UFCR_TXTL(X)	((X) <<10)	// Transmitter Trigger Level
#define	     UFCR_RFDIV(x)	((6 - ((x) % 7)) << 7) // Reference Frequency Divider
#define	     UFCR_DCEDTE	(1<< 6)	// DCE/DTE Mode Select
#define	     UFCR_RXTL(X)	((X) << 0)	// Receiver Trigger Level
#define	UART_USR1	0x94	// Status Register 1
#define	     USR1_PARITYERR	(1<<15)	// Parity Error Interrupt Flag
#define	     USR1_RTSS		(1<<14)	// RTS_B Pin Status
#define	     USR1_TRDY		(1<<13)	// Transmitter Ready Interrupt/DMA Flag
#define	     USR1_RTSD		(1<<12)	// RTS Delta
#define	     USR1_ESCF		(1<<11)	// Escape Sequence Interrupt Flag
#define	     USR1_FRAMERR	(1<<10)	// Frame Error Interrupt Flag
#define	     USR1_RRDY		(1<< 9)	// Receiver Ready Interrupt/DMA Flag
#define	     USR1_AGTIM		(1<< 8)	// Ageing Timer Interrupt Flag
#define	     USR1_DTRD		(1<< 7)	// DTR Delta
#define	     USR1_RXDS		(1<< 6)	// Receiver Idle Interrupt Flag
#define	     USR1_AIRINT	(1<< 5)	// Async IR Wake Interrupt Flag
#define	     USR1_AWAKE		(1<< 4)	// Aysnc Wake Interrupt Flag
#define	UART_USR2	0x98	// Status Register 2
#define	     USR2_ADET		(1<<15)	// Automatic Baud Rate Detect Complete
#define	     USR2_TXFE		(1<<14)	// Transmit Buffer FIFO Empty
#define	     USR2_DTRF		(1<<13)	// DTR Edge Interrupt Flag
#define	     USR2_IDLE		(1<<12)	// Idle Condition
#define	     USR2_ACST		(1<<11)	// Autobaud Counter Stopped
#define	     USR2_RIDELT	(1<<10)	// Ring Indicator Delta
#define	     USR2_RIIN		(1<< 9)	// Ring Indicator Input
#define	     USR2_IRINT		(1<< 8)	// Serial Infrared Interrupt Flag
#define	     USR2_WAKE		(1<< 7)	// Wake
#define	     USR2_DCDDELT	(1<< 6)	// DCD Delta
#define	     USR2_DCDIN		(1<< 6)	// DCD Input
#define	     USR2_RTSF		(1<< 4)	// RTS Edge Interrupt Flag
#define	     USR2_TXDC		(1<< 3)	// Transmitter Complete
#define	     USR2_BRCD		(1<< 2)	// Break Condition
#define	     USR2_ORE		(1<< 1)	// Overrun Error
#define	     USR2_RDR		(1<< 0)	// Recv Data Ready
#define	UART_UESC	0x9C	// Escape Character Register
#define	UART_UTIM	0xA0	// Escape Timer Register
#define	UART_UBIR	0xA4	// BRM Incremental Register
#define	UART_UBMR	0xA8	// BRM Modulator Register
#define	UART_UBRC	0xAC	// Baud Rate Count Register
#define	UART_ONEMS	0xB0	// One Millisecond Register
#define	UART_UTS	0xB4	// Test Register
#define	     UTS_FRCPERR	(1<<13)	// Force Parity Error
#define	     UTS_LOOP		(1<<12)	// Loop Tx and Rx
#define	     UTS_TXEMPTY	(1<< 6)	// TxFIFO Empty
#define	     UTS_RXEMPTY	(1<< 5)	// RxFIFO Empty
#define	     UTS_TXFULL		(1<< 4)	// TxFIFO Full
#define	     UTS_RXFULL		(1<< 3)	// RxFIFO Full
#define	     UTS_SOFTRST	(1<< 0)	// Software Reset
#define	UART_UMCR	0xB8	// RS485 Mode Control Register

#endif
