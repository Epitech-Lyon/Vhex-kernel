#include <kernel/context.h>
#include <kernel/hardware/keysc.h>
#include <kernel/hardware/intc.h>

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
}

void fx9860_context_restore(fx9860_context_t *context)
{
	// Restore Key Scan (KEYSC) context.
	SH7305_KEYSC.CONTROL.WORD = context->keyboard.control;
	SH7305_KEYSC.AUTOFIX.WORD = context->keyboard.autofix;
	SH7305_KEYSC.SCAN_MODE.WORD = context->keyboard.scan_mode;
	SH7305_KEYSC.SCAN_STATE.WORD = context->keyboard.scan_state;
	SH7305_KEYSC.INTERRUPT.WORD = context->keyboard.interrupt;
	SH7305_KEYSC.SCAN_WAIT.WORD = context->keyboard.scan_wait;
	SH7305_KEYSC.SCAN_INTERVAL = context->keyboard.scan_interval;
	SH7305_KEYSC.KYOUTDR.WORD = context->keyboard.kyoutdr;
	SH7305_KEYSC.KYINDR.WORD = context->keyboard.kyindr;

	// Restore Interrupt Controller (INTC)
	SH7305_INTC.IPRA.WORD = context->intc.ipra;
	SH7305_INTC.IPRB.WORD = context->intc.iprb;
	SH7305_INTC.IPRC.WORD = context->intc.iprc;
	SH7305_INTC.IPRD.WORD = context->intc.iprd;
	SH7305_INTC.IPRE.WORD = context->intc.ipre;
	SH7305_INTC.IPRF.WORD = context->intc.iprf;
	SH7305_INTC.IPRG.WORD = context->intc.iprg;
	SH7305_INTC.IPRH.WORD = context->intc.iprh;
	SH7305_INTC.IPRI.WORD = context->intc.ipri;
	SH7305_INTC.IPRJ.WORD = context->intc.iprj;
	SH7305_INTC.IPRK.WORD = context->intc.iprk;
	SH7305_INTC.IPRL.WORD = context->intc.iprl;
}
