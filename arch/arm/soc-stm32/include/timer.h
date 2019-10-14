#ifndef	_MACH_TIMER_H_
#define	_MACH_TIMER_H_


#define	TIM2_BASE	0x40000000
#define	TIM3_BASE	0x40000400
#define	TIM4_BASE	0x40000800
#define	TIM5_BASE	0x40000C00
#define	TIM6_BASE	0x40001000
#define	TIM7_BASE	0x40001400
#define	TIM12_BASE	0x40001800
#define	TIM13_BASE	0x40001C00
#define	TIM14_BASE	0x40002000
#define	TIM1_BASE	0x40010000
#define	TIM8_BASE	0x40010400
#define	TIM9_BASE	0x40014000
#define	TIM10_BASE	0x40014400
#define	TIM11_BASE	0x40014800
#define	TIM_SIZE	     0x400


#define	TIM2_IRQ	28
#define	TIM3_IRQ	29
#define	TIM4_IRQ	30
#define	TIM5_IRQ	50
#define	TIM7_IRQ	55


#define	TIM_CR1		0x00
#define	TIM_CR1_ARPE		(1 << 7)
#define	TIM_CR1_OPM		(1 << 3)
#define	TIM_CR1_CEN		(1 << 0)

#define	TIM_DIER	0x0C
#define	TIM_DIER_UIE		(1 << 0)

#define	TIM_SR		0x10
#define	TIM_SR_UIF		(1 << 0)

#define	TIM_EGR		0x14
#define	TIM_EGR_UG		(1 << 0)

#define	TIM_CNT		0x24

#define	TIM_PSC		0x28
#define	TIM_PSC_MAX		0xffff

#define	TIM_ARR		0x2C

#endif
