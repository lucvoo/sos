#include <init.h>
#include <io.h>


#define	DBGMCU_BASE	((void __iomem *) 0xE0042000)
#define	DBGMCU_CR	(DBGMCU_BASE + 0x04)
#define	DBG_STANDBY		(1 << 2)
#define	DBG_STOP		(1 << 1)
#define	DBG_SLEEP		(1 << 0)


#ifdef	CONFIG_LOWPOWER_DEBUG
static void __init debug_init(void)
{
	iowrite32(DBGMCU_CR, DBG_STANDBY | DBG_STOP | DBG_SLEEP);
}
pure_initcall(debug_init);
#endif
