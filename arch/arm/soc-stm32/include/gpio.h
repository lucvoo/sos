#ifndef	_MACH_GPIO_H_
#define	_MACH_GPIO_H_


#define GPIO_MODER	0x00
#define GPIO_MODE(PIN, VAL)	((VAL) << ((PIN)*2))
#define GPIO_MODE_IN(PIN)	GPIO_MODE(PIN, 0x00)
#define GPIO_MODE_OUT(PIN)	GPIO_MODE(PIN, 0x01)
#define GPIO_MODE_AF(PIN)	GPIO_MODE(PIN, 0x02)
#define GPIO_MODE_AN(PIN)	GPIO_MODE(PIN, 0x03)
#define GPIO_MODE_MASK(PIN)	GPIO_MODE(PIN, 0x03)

#define GPIO_OTYPER	0x04
#define GPIO_OTYPE(PIN, VAL)	((VAL) << (PIN))
#define GPIO_OTYPE_PP(PIN)	GPIO_OTYPE(PIN, 0x00)
#define GPIO_OTYPE_OD(PIN)	GPIO_OTYPE(PIN, 0x01)

#define GPIO_SPEEDR	0x08
#define GPIO_SPEED(PIN, VAL)	((VAL) << ((PIN)*2))
#define GPIO_SPEED_LOW(PIN)	GPIO_SPEED(PIN, 0x00)
#define GPIO_SPEED_MED(PIN)	GPIO_SPEED(PIN, 0x01)
#define GPIO_SPEED_FAST(PIN)	GPIO_SPEED(PIN, 0x02)
#define GPIO_SPEED_HIGH(PIN)	GPIO_SPEED(PIN, 0x03)

#define GPIO_PUPDR	0x0C
#define GPIO_PUPD(PIN, VAL)	((VAL) << ((PIN)*2))
#define GPIO_PUPD_NO(PIN)	GPIO_PUPD(PIN, 0x00)
#define GPIO_PUPD_UP(PIN)	GPIO_PUPD(PIN, 0x01)
#define GPIO_PUPD_DOWN(PIN)	GPIO_PUPD(PIN, 0x02)

#define GPIO_IDR	0x10
#define GPIO_ID(PIN)		(1 << (PIN))

#define GPIO_ODR	0x14
#define GPIO_OD(PIN, VAL)	((VAL) << (PIN))

#define GPIO_BSRR	0x18
#define GPIO_BSET(PIN)		(1 << ((PIN)+0))
#define GPIO_BCLR(PIN)		(1 << ((PIN)+16))

#define GPIO_LCKR	0x1C
#define GPIO_LCK(PIN)		(1 << (PIN))
#define GPIO_LCKK		(1 << 16)

#define GPIO_AFR(PIN)	(0x20 + ((PIN)&8)/2)
#define GPIO_AF(PIN,AF)		((AF) << (((PIN)&7)*4))

#endif
