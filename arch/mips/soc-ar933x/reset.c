#include <soc/reset.h>
#include <soc/ar71xx_regs.h>
#include <arch/iomap.h>
#include <init.h>
#include <lock.h>
#include <io.h>


static struct lock   reset_lock = LOCK_UNLOCKED;

#define reset_iobase	__ioremap(AR71XX_RESET_BASE)


void ath79_reset_set(u32 mask)
{
	void __iomem *ioaddr = reset_iobase + AR933X_RESET_REG_RESET;
	ulong flags;
	u32 old, new;

	flags = lock_acq_save(&reset_lock);
	old = ioread32(ioaddr);
	new = old | mask;
	iowrite32(ioaddr, new);
	lock_rel_rest(&reset_lock, flags);
}

void ath79_reset_clr(u32 mask)
{
	void __iomem *ioaddr = reset_iobase + AR933X_RESET_REG_RESET;
	ulong flags;
	u32 old, new;

	flags = lock_acq_save(&reset_lock);
	old = ioread32(ioaddr);
	new = old & ~mask;
	iowrite32(ioaddr, new);
	lock_rel_rest(&reset_lock, flags);
}
