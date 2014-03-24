#ifndef _HW_AM335X_IRQS_H_
#define _HW_AM335X_IRQS_H_

#define IRQ_3PGSWMISC0		43	// CPSW (Ethernet) MISC_PULSE
#define IRQ_3PGSWRXINT0		41	// CPSW (Ethernet) RX_PULSE
#define IRQ_3PGSWRXTHR0		40	// CPSW (Ethernet) RX_THRESH_PULSE
#define IRQ_3PGSWTXINT0		42	// CPSW (Ethernet) TX_PULSE
#define IRQ_ADC_TSC_GENINT	16	// ADC_TSC (Touchscreen Controller)
#define IRQ_ADC_TSC_PENINT	115	// ADC_TSC
#define IRQ_BENCH		3	// MPU Subsystem Internal
#define IRQ_COMMRX		2	// MPU Subsystem Internal
#define IRQ_COMMTX		1	// MPU Subsystem Internal
#define IRQ_DCAN0_INT0		52	// DCAN0
#define IRQ_DCAN0_INT1		53	// DCAN0
#define IRQ_DCAN0_PARITY	54	// DCAN0
#define IRQ_DCAN1_INT0		55	// DCAN1
#define IRQ_DCAN1_INT1		56	// DCAN1
#define IRQ_DCAN1_PARITY	57	// DCAN1
#define IRQ_DDRERR0		101	// EMIF
#define IRQ_DMA_INTR_PIN0	123	// External DMA/Interrupt Pin0 (xdma_event_intr0)
#define IRQ_DMA_INTR_PIN1	124	// External DMA/Interrupt Pin1 (xdma_event_intr1)
#define IRQ_DMA_INTR_PIN2	90	// External DMA/Interrupt Pin2 (xdma_event_intr2)
#define IRQ_eCAP0INT		31	// eCAP0 event/interrupt
#define IRQ_eCAP1INT		47	// eCAP1 (PWM Subsystem)
#define IRQ_eCAP2INT		61	// eCAP2 (PWM Subsystem)
#define IRQ_EDMACOMPINT		12	// TPCC (EDMA)
#define IRQ_EDMAERRINT		14	// TPCC (EDMA)
#define IRQ_EDMAMPERR		13	// TPCC (EDMA)
#define IRQ_ELM_IRQ		4	// ELM
#define IRQ_EMUINT		0	// MPU Subsystem Internal
#define IRQ_ePWM0INT		86	// eHRPWM0 (PWM Subsystem)
#define IRQ_ePWM0_TZINT		58	// eHRPWM0 TZ interrupt (PWM Subsystem)
#define IRQ_ePWM1INT		87	// eHRPWM1 (PWM Subsystem)
#define IRQ_ePWM1_TZINT		59	// eHRPWM1 TZ interrupt (PWM Subsystem)
#define IRQ_ePWM2INT		39	// eHRPWM2 (PWM Subsystem)
#define IRQ_ePWM2_TZINT		60	// eHRPWM2 TZ interrupt (PWM Subsystem)
#define IRQ_eQEP0INT		79	// eQEP0 (PWM Subsystem)
#define IRQ_eQEP1INT		88	// eQEP1 (PWM Subsystem)
#define IRQ_eQEP2INT		89	// eQEP2 (PWM Subsystem)
#define IRQ_GFXINT		37	// SGX530
#define IRQ_GPIOINT0A		96	// GPIO 0
#define IRQ_GPIOINT0B		97	// GPIO 0
#define IRQ_GPIOINT1A		98	// GPIO 1
#define IRQ_GPIOINT1B		99	// GPIO 1
#define IRQ_GPIOINT2A		32	// GPIO 2
#define IRQ_GPIOINT2B		33	// GPIO 2
#define IRQ_GPIOINT3A		62	// GPIO 3
#define IRQ_GPIOINT3B		63	// GPIO 3
#define IRQ_GPMCINT		100	// GPMC
#define IRQ_I2C0INT		70	// I2C0
#define IRQ_I2C1INT		71	// I2C1
#define IRQ_I2C2INT		30	// I2C2
#define IRQ_L3APPINT		10	// L3
#define IRQ_L3DEBUG		9	// L3
#define IRQ_LCDCINT		36	// LCDC
#define IRQ_M3_TXEV		78	// Wake M3 Subsystem
#define IRQ_MBINT0		77	// Mailbox0 (mail_u0_irq)
#define IRQ_MCARXINT0		81	// McASP0
#define IRQ_MCARXINT1		83	// McASP1
#define IRQ_MCATXINT0		80	// McASP0
#define IRQ_MCATXINT1		82	// McASP1
#define IRQ_McSPI0INT		65	// McSPI0
#define IRQ_McSPI1INT		125	// McSPI1
#define IRQ_MMCSD0INT		64	// MMCSD0
#define IRQ_MMCSD1INT		28	// MMCSD1
#define IRQ_MMCSD2INT		29	// MMCSD2
#define IRQ_NMI			7	// External Pin (active low)(1)
#define IRQ_PRCMINT		11	// PRCM
#define IRQ_PRU_ICSS_EVTOUT0	20	// pr1_host[0]
#define IRQ_PRU_ICSS_EVTOUT1	21	// pr1_host[1]
#define IRQ_PRU_ICSS_EVTOUT2	22	// pr1_host[2]
#define IRQ_PRU_ICSS_EVTOUT3	23	// pr1_host[3]
#define IRQ_PRU_ICSS_EVTOUT4	24	// pr1_host[4]
#define IRQ_PRU_ICSS_EVTOUT5	25	// pr1_host[5]
#define IRQ_PRU_ICSS_EVTOUT6	26	// pr1_host[6]
#define IRQ_PRU_ICSS_EVTOUT7	27	// pr1_host[7]
#define IRQ_RTCALARMINT		76	// RTC
#define IRQ_RTCINT		75	// RTC
#define IRQ_SMRFLX_Core		121	// Smart Reflex 1
#define IRQ_SMRFLX_MPU		120	// Smart Reflex 0
#define IRQ_TCERRINT0		112	// TPTC0
#define IRQ_TCERRINT1		113	// TPTC1
#define IRQ_TCERRINT2		114	// TPTC2
#define IRQ_TINT0		66	// Timer0
#define IRQ_TINT1_1MS		67	// DMTIMER_1ms
#define IRQ_TINT2		68	// DMTIMER2
#define IRQ_TINT3		69	// DMTIMER3
#define IRQ_TINT4		92	// DMTIMER4
#define IRQ_TINT5		93	// DMTIMER5
#define IRQ_TINT6		94	// DMTIMER6
#define IRQ_TINT7		95	// DMTIMER7
#define IRQ_UART0INT		72	// UART0
#define IRQ_UART1INT		73	// UART1
#define IRQ_UART2INT		74	// UART2
#define IRQ_UART3INT		44	// UART3
#define IRQ_UART4INT		45	// UART4
#define IRQ_UART5INT		46	// UART5
#define IRQ_USBINT0		18	// USBSS
#define IRQ_USBINT1		19	// USBSS
#define IRQ_USBSSINT		17	// USBSS
#define IRQ_USBWAKEUP		34	// USBSS
#define IRQ_WDT1INT		91	// WDTIMER1 (Public Watchdog)

#endif
