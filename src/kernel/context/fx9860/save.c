#include <kernel/context.h>
#include <kernel/hardware/keysc.h>
#include <kernel/hardware/power.h>
#include <kernel/hardware/intc.h>
#include <kernel/hardware/tmu.h>
#include <kernel/vbr.h>

void fx9860_context_save(fx9860_context_t *context)
{
	// Save Interrupt Controller (INTC)
	context->intc.ipra = SH7305_INTC.IPRA.WORD;
	context->intc.iprb = SH7305_INTC.IPRB.WORD;
	context->intc.iprc = SH7305_INTC.IPRC.WORD;
	context->intc.iprd = SH7305_INTC.IPRD.WORD;
	context->intc.ipre = SH7305_INTC.IPRE.WORD;
	context->intc.iprf = SH7305_INTC.IPRF.WORD;
	context->intc.iprg = SH7305_INTC.IPRG.WORD;
	context->intc.iprh = SH7305_INTC.IPRH.WORD;
	context->intc.ipri = SH7305_INTC.IPRI.WORD;
	context->intc.iprj = SH7305_INTC.IPRJ.WORD;
	context->intc.iprk = SH7305_INTC.IPRK.WORD;
	context->intc.iprl = SH7305_INTC.IPRL.WORD;

	// Save Key Scan (KEYSC) context.
	context->keyboard.control = SH7305_KEYSC.CONTROL.WORD;
	context->keyboard.autofix = SH7305_KEYSC.AUTOFIX.WORD;
	context->keyboard.scan_mode = SH7305_KEYSC.SCAN_MODE.WORD;
	context->keyboard.scan_state = SH7305_KEYSC.SCAN_STATE.WORD;
	context->keyboard.interrupt = SH7305_KEYSC.INTERRUPT.WORD;
	context->keyboard.scan_wait = SH7305_KEYSC.SCAN_WAIT.WORD;
	context->keyboard.scan_interval = SH7305_KEYSC.SCAN_INTERVAL;
	context->keyboard.kyoutdr = SH7305_KEYSC.KYOUTDR.WORD;
	context->keyboard.kyindr = SH7305_KEYSC.KYINDR.WORD;

	// Save power context.
	context->power.stbcr = SH7305_POWER.STBCR.LONG_WORD;
	context->power.mstpcr0 = SH7305_POWER.MSTPCR0.LONG_WORD;
	context->power.mstpcr1 = SH7305_POWER.MSTPCR1.LONG_WORD;
	context->power.mstpcr2 = SH7305_POWER.MSTPCR2.LONG_WORD;

	// Save TMU context.
	context->tmu.tstr = SH7305_TMU.TSTR.BYTE;
	SH7305_TMU.TSTR.BYTE = 0x00; // stop all timer.
	context->tmu.timer[0].tcor = SH7305_TMU.TIMER[0].TCOR;
	context->tmu.timer[0].tcnt = SH7305_TMU.TIMER[0].TCNT;
	context->tmu.timer[0].tcr  = SH7305_TMU.TIMER[0].TCR.WORD;
	context->tmu.timer[1].tcor = SH7305_TMU.TIMER[1].TCOR;
	context->tmu.timer[1].tcnt = SH7305_TMU.TIMER[1].TCNT;
	context->tmu.timer[1].tcr  = SH7305_TMU.TIMER[1].TCR.WORD;
	context->tmu.timer[2].tcor = SH7305_TMU.TIMER[2].TCOR;
	context->tmu.timer[2].tcnt = SH7305_TMU.TIMER[2].TCNT;
	context->tmu.timer[2].tcr  = SH7305_TMU.TIMER[2].TCR.WORD;

	// Save current VBR.
	context->vbr = (uint32_t)vbr_get();
}

