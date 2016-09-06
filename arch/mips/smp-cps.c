#include <arch/cpc.h>
#include <smp/init.h>
#include <smp/ops.h>
#include <soc/smp.h>
#include <memory.h>
#include <thread.h>
#include <delay.h>
#include <smp.h>
#include <io.h>


static void cps_init_vpes(void)
{
}

static void cps_init_core(void)
{
	// disable others threads & vpes
	asm volatile("dmt");
	asm volatile("dvpe");

	cps_init_vpes();
}

static void cps_boot_core_cpc(uint core)
{
	mips_cpc_get(core);

	// reset the core and wait it's up
	mips_cpc_co_write(CPC_CX_CMD, CPC_CX_CMD_RESET);

	for (int tries = 1000; ; tries--) {
		uint stat = mips_cpc_co_read(CPC_CX_STAT_CFG);

		if (CPC_CX_SEQSTATE(stat) == CPC_CX_SEQSTATE_U6) {
			break;	// coherent execution: core is up
		}

		udelay(10);

		if (tries < 0) {
			pr_warn("Booting core %d ... (stat = %x (%08x))\n",
				core, CPC_CX_SEQSTATE(stat), stat);
			mdelay(1000);
		}
	}

	mips_cpc_put();
}

static void cps_boot_core(uint core)
{
	mips_cm_get(core);

	// ensure coherence is off
	mips_gcr_co_write(GCR_CX_COHERENCE, 0);

	// set the entry vector (on the uncached region)
	mips_gcr_co_write(GCR_CX_RESET_BASE, virt_to_kseg1(__smp_entry));
	mips_gcr_co_write(GCR_CX_RESET_EXT, GCR_CX_RESET_EXT_UEB);

	// give the core access to the GCRs
	mips_gcr_set(GCR_ACCESS, (1 << core));

	// FIXME: test if CPC present
	cps_boot_core_cpc(core);

	mips_cm_put();
}

////////////////////////////////////////////////////////////////////////////////
static int smp_cps_init(void)
{
	// FIXME: check if NR_CORES match reality

	// set cache coherency
	c0_chgbits(c0_config, CFG0_K0_MASK, CFG0_K0_CWB);

	// init core 0
	cps_init_core();

	// set coherence with everything
	mips_gcr_cl_write(GCR_CX_COHERENCE, GCR_CX_COHERENCE_ALL);

	return 0;
}

static int smp_cps_boot_cpu(struct thread *idle, uint cpu)
{
	uint core = __coreid(cpu);
	uint vpid = __vpeid(cpu);
	int rc = 0;

	if (vpid == 0) {
		cps_boot_core(core);
		goto end;
	}

	// FIXME: the simplest would be to let each cores to boot
	//        its 'secondary VPEs'
end:
	return rc;
}

static int smp_cps_init_cpu(uint cpu)
{
	return 0;
}

struct smp_ops smp_ops = {
	.init	  = smp_cps_init,
	.boot_cpu = smp_cps_boot_cpu,
	.init_cpu = smp_cps_init_cpu,
};
