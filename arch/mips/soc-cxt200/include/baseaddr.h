#ifndef	_SOC_BASEADDR_H_
#define	_SOC_BASEADDR_H_

#define DRAM_BASE	0x00000000	// DDRexternalMemory(256Mbytes)
//			0x10000000	// Reserved
#define I2C0_BASE	0x18100000	// I2C0
#define I2C1_BASE	0x18100200	// I2C1
#define I2C2_BASE	0x18100400	// I2C2
#define I2C3_BASE	0x18100600	// I2C3
#define I2S_IN_BASE	0x18100800	// I2S Audio in
#define I2S_OUT_BASE	0x18100A00	// I2S Audio out
#define PAUDIO_OUT_BASE	0x18100C00	// Parallel Audio out
#define SPDIF_OUT_BASE	0x18100D00	// SPDIF Audio out
#define SPDIF_IN_BASE	0x18100E00	// SPDIF Audio in
#define SPI0_BASE	0x18100F00	// SPFI Master0
#define SPI1_BASE	0x18101000	// SPFI Master1
#define HASH0_BASE	0x18101100	// Hash Accelerator
#define SPI_SLAVE_BASE	0x18101200	// SPI Slave
#define DAC_PWM_BASE	0x18101300	// PWM DAC
#define UART0_BASE	0x18101400	// UART 0
#define UART1_BASE	0x18101500	// UART 1
#define ADC_AUX_BASE	0x18101600	// Aux ADC
#define SOCIF_BASE	0x18101800	// SOCIF internal
#define GPIO_BASE	0x18101C00	// GPIO Control
#define TIMER_BASE	0x18102000	// Timer
#define WATCHDOG_BASE	0x18102100	// Watchdog
#define IR_BASE		0x18102200	// IR
#define ETIMER_BASE	0x18102300	// Event Timer Stamp
//			0x18102400	// Event Timer Wrap
#define USB_BASE	0x18120000	// USB
//			0x1813C000	// Reserved
#define ETHERNET_BASE	0x18140000	// Ethernet
#define SDC_BASE	0x18142000	// SD Host
//			0x18142400	// Reserved
#define DMAC_BASE	0x18143000	// MDC (DMAC)
#define MCLCK_CTRL_BASE	0x18144000	// Main Clock Control
#define PCLCK_CTRL_BASE	0x18144800	// Peripheral Clock Control
#define PERCTRL_BASE	0x18148000	// Peripheral General Control
#define TOP_LEVEL_BASE	0x18149000	// Top Level General
#define DDR_PERF_BASE	0x18149400	// DDR Performance monitor
#define HASH1_BASE	0x18149600	// Hash Accelerator
#define DDR_CTRL_BASE	0x18180000	// DDR Controller
#define DDR_PHY_BASE	0x18180800	// DDR PHY
//			0x18181000	// Reserved
#define SYSINF_BASE	0x18190000	// Sys info
//			0x18190800	// Reserved
#define RPU_SYSBUS_BASE	0x18480000	// RPU system bus
#define RPU_SLABUS_BASE	0x18489000	// RPU system slave bus
#define RPU_SOC_BASE	0x184B8000	// RPU SoC Wrapper
#define RPU_PAD_BASE	0x184BA000	// RPU Pad Control
#define RPU_PERBUS_BASE	0x184C0000	// RPU Peripheral Bus
#define RPU_BTXCHG_BASE	0x18540000	// RPU BT Exchange
//			0x18580000	// Reserved
#define RPU_GRAM_BASE	0x1A000000	// RPU GRAM
//			0x1A066CC0	// Reserved
#define SRAM_BASE	0x1B000000	// System SRAM
//			0x1B010000	// Reserved
#define BOOT_ROM_BASE	0x1FC00000	// Boot ROM
//			0x1FC04000	// Reserved

#endif
