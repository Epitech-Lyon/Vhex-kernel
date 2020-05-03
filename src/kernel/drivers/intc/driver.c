#include <kernel/hardware/intc.h>
#include <kernel/bits/driver.h>

// Internal hardware context
struct __sh7305_intc_context intc_context;

static void sh7305_intc_driver_install(void)
{
	// Dump current context
	intc_context.ipra = SH7305_INTC.IPRA.WORD;
	intc_context.iprb = SH7305_INTC.IPRB.WORD;
	intc_context.iprc = SH7305_INTC.IPRC.WORD;
	intc_context.iprd = SH7305_INTC.IPRD.WORD;
	intc_context.ipre = SH7305_INTC.IPRE.WORD;
	intc_context.iprf = SH7305_INTC.IPRF.WORD;
	intc_context.iprg = SH7305_INTC.IPRG.WORD;
	intc_context.iprh = SH7305_INTC.IPRH.WORD;
	intc_context.ipri = SH7305_INTC.IPRI.WORD;
	intc_context.iprj = SH7305_INTC.IPRJ.WORD;
	intc_context.iprk = SH7305_INTC.IPRK.WORD;
	intc_context.iprl = SH7305_INTC.IPRL.WORD;
	intc_context.imr[0] = SH7305_INTC.IMR0.BYTE;
	intc_context.imr[1] = SH7305_INTC.IMR1.BYTE;
	intc_context.imr[2] = SH7305_INTC.IMR2.BYTE;
	intc_context.imr[3] = SH7305_INTC.IMR3.BYTE;
	intc_context.imr[4] = SH7305_INTC.IMR4.BYTE;
	intc_context.imr[5] = SH7305_INTC.IMR5.BYTE;
	intc_context.imr[6] = SH7305_INTC.IMR6.BYTE;
	intc_context.imr[7] = SH7305_INTC.IMR7.BYTE;
	intc_context.imr[8] = SH7305_INTC.IMR8.BYTE;
	intc_context.imr[9] = SH7305_INTC.IMR9.BYTE;
	intc_context.imr[10] = SH7305_INTC.IMR10.BYTE;
	intc_context.imr[11] = SH7305_INTC.IMR11.BYTE;
	intc_context.icr[0] = SH7305_INTC.ICR0.WORD;
	intc_context.icr[1] = SH7305_INTC.ICR1.WORD;
	intc_context.intpri00 = SH7305_INTC.INTPRI00.LONG_WORD;
	intc_context.intreq00 = SH7305_INTC.INTREQ00.BYTE;
	intc_context.intmsk00 = SH7305_INTC.INTMSK00.BYTE;
	intc_context.intmskclr00 = SH7305_INTC.INTMSKCLR00.BYTE;
	intc_context.nmifcr = SH7305_INTC.NMIFCR.WORD;
	intc_context.usermsk = SH7305_INTC.USERMSK.LONG_WORD;

	// Initialize Interrupt Controller (INTC).
	SH7305_INTC.IPRA.WORD = 0x0000;
	SH7305_INTC.IPRB.WORD = 0x0000;
	SH7305_INTC.IPRC.WORD = 0x0000;
	SH7305_INTC.IPRD.WORD = 0x0000;
	SH7305_INTC.IPRE.WORD = 0x0000;
	SH7305_INTC.IPRF.WORD = 0x0000;
	SH7305_INTC.IPRG.WORD = 0x0000;
	SH7305_INTC.IPRH.WORD = 0x0000;
	SH7305_INTC.IPRI.WORD = 0x0000;
	SH7305_INTC.IPRJ.WORD = 0x0000;
	SH7305_INTC.IPRK.WORD = 0x0000;
	SH7305_INTC.IPRL.WORD = 0x0000;

	SH7305_INTC.IMR0.BYTE = 0xff;
	SH7305_INTC.IMR1.BYTE = 0xff;
	SH7305_INTC.IMR2.BYTE = 0xff;
	SH7305_INTC.IMR3.BYTE = 0xff;
	SH7305_INTC.IMR4.BYTE = 0xff;
	SH7305_INTC.IMR5.BYTE = 0xff;
	SH7305_INTC.IMR6.BYTE = 0xff;
	SH7305_INTC.IMR7.BYTE = 0xff;
	SH7305_INTC.IMR8.BYTE = 0xff;
	SH7305_INTC.IMR9.BYTE = 0xff;
	SH7305_INTC.IMR10.BYTE = 0xff;
	SH7305_INTC.IMR11.BYTE = 0xff;

	SH7305_INTC.ICR0.NMIL = 0;			// NMI input level to low
	SH7305_INTC.ICR0.MAI  = 1;			// disable NMI interrupt when input is low
	SH7305_INTC.ICR0.NMIB = 0;			// keep NMI interrupt pending when BL bit is set
	SH7305_INTC.ICR0.NMIE = 0;			// select faling edge interrupt
	SH7305_INTC.ICR0.LVLMODE = 1;			// retain interrupt request
	SH7305_INTC.ICR1.WORD = 0x0000;			// select (all) interrupt request on falling edge
	SH7305_INTC.INTPRI00.LONG_WORD = 0x00000000;	// disable all interrupt request
	SH7305_INTC.INTMSK00.BYTE = 0x00;		// mask all interrupt request
	SH7305_INTC.NMIFCR.NMIFL = 0;			// clear NMI flags

	SH7305_INTC.USERMSK.SECRET = 0xa5;		// secret
	SH7305_INTC.USERMSK.UIMASK = 0b0000;		// enable all interrupt in user mode (not used here)
}

static void sh7305_intc_driver_uninstall(void)
{
	SH7305_INTC.IPRA.WORD = intc_context.ipra;
	SH7305_INTC.IPRB.WORD = intc_context.iprb;
	SH7305_INTC.IPRC.WORD = intc_context.iprc;
	SH7305_INTC.IPRD.WORD = intc_context.iprd;
	SH7305_INTC.IPRE.WORD = intc_context.ipre;
	SH7305_INTC.IPRF.WORD = intc_context.iprf;
	SH7305_INTC.IPRG.WORD = intc_context.iprg;
	SH7305_INTC.IPRH.WORD = intc_context.iprh;
	SH7305_INTC.IPRI.WORD = intc_context.ipri;
	SH7305_INTC.IPRJ.WORD = intc_context.iprj;
	SH7305_INTC.IPRK.WORD = intc_context.iprk;
	SH7305_INTC.IPRL.WORD = intc_context.iprl;
	SH7305_INTC.IMR0.BYTE = intc_context.imr[0];
	SH7305_INTC.IMR1.BYTE = intc_context.imr[1];
	SH7305_INTC.IMR2.BYTE = intc_context.imr[2];
	SH7305_INTC.IMR3.BYTE = intc_context.imr[3];
	SH7305_INTC.IMR4.BYTE = intc_context.imr[4];
	SH7305_INTC.IMR5.BYTE = intc_context.imr[5];
	SH7305_INTC.IMR6.BYTE = intc_context.imr[6];
	SH7305_INTC.IMR7.BYTE = intc_context.imr[7];
	SH7305_INTC.IMR8.BYTE = intc_context.imr[8];
	SH7305_INTC.IMR9.BYTE = intc_context.imr[9];
	SH7305_INTC.IMR10.BYTE = intc_context.imr[10];
	SH7305_INTC.IMR11.BYTE = intc_context.imr[11];
	SH7305_INTC.IMCR0.BYTE = ~intc_context.imr[0];
	SH7305_INTC.IMCR1.BYTE = ~intc_context.imr[1];
	SH7305_INTC.IMCR2.BYTE = ~intc_context.imr[2];
	SH7305_INTC.IMCR3.BYTE = ~intc_context.imr[3];
	SH7305_INTC.IMCR4.BYTE = ~intc_context.imr[4];
	SH7305_INTC.IMCR5.BYTE = ~intc_context.imr[5];
	SH7305_INTC.IMCR6.BYTE = ~intc_context.imr[6];
	SH7305_INTC.IMCR7.BYTE = ~intc_context.imr[7];
	SH7305_INTC.IMCR8.BYTE = ~intc_context.imr[8];
	SH7305_INTC.IMCR9.BYTE = ~intc_context.imr[9];
	SH7305_INTC.IMCR10.BYTE = intc_context.imr[10];
	SH7305_INTC.IMCR11.BYTE = intc_context.imr[11];
	SH7305_INTC.ICR0.WORD = intc_context.icr[0];
	SH7305_INTC.ICR1.WORD = intc_context.icr[1];
	SH7305_INTC.INTPRI00.LONG_WORD = intc_context.intpri00;
	SH7305_INTC.INTREQ00.BYTE = intc_context.intreq00;
	SH7305_INTC.INTMSK00.BYTE = intc_context.intmsk00;
	SH7305_INTC.INTMSKCLR00.BYTE = intc_context.intmskclr00;
	SH7305_INTC.NMIFCR.WORD = intc_context.nmifcr;
	SH7305_INTC.USERMSK.LONG_WORD = intc_context.usermsk;
}

// Create driver object
VHEX_DRIVER(2, driver_intc_sh7305) = {
	.arch = MPU_SH7305,
	.install = &sh7305_intc_driver_install,
	.uninstall = &sh7305_intc_driver_uninstall,
	.name = "cpg"
};
