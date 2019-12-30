#include <stdint.h>
#include <stddef.h>
#include <kernel/hardware/keysc.h>
#include <kernel/hardware/intc.h>
#include <kernel/hardware/power.h>
#include <kernel/hardware/tmu.h>
#include <kernel/vbr.h>

void vhex_context_set(void)
{
	extern uint32_t vhex_vbr;

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

	// Switch VBR
	// @note:
	// 	The VBR swtich *SHOULD* be atomic
	// because one exepction / interrupt will
	// crash the calculator.
	vbr_set(&vhex_vbr);

	// Configure Key Scan (KEYSC)
	SH7305_INTC.IMR5.KEYI = 1;			// Mask KEYSC interrupt.
	SH7305_INTC.IPRF.KEYSC = 0;			// Disable KEYSC interrupt.
	SH7305_TMU.TIMER[0].TCOR = 0xffffffff;		// Disable Timer 2
	SH7305_KEYSC.CONTROL.KEYIE = 1;			// Enable KEYSC interrupt.
	SH7305_KEYSC.CONTROL.INTMODE = 0b00;		// Use default (?) interrupt mode.
	SH7305_KEYSC.AUTOFIX.ENABLE = 1;		// Enable key bounce fix (?)
	SH7305_KEYSC.AUTOFIX.UNKNOWN0 = 0b100;		// Unknown, use Casio's value. (autofix)
	SH7305_KEYSC.AUTOFIX.UNKNOWN1 = 0b10;		// Unknown, use Casio's value. (autofix)
	SH7305_KEYSC.SCAN_MODE.UNKNOWN = 0b001;		// Unknown, use Casio's value. (scan mode)
	//SH7305_KEYSC.INTERRUPT.KYCPU_IE = 0b0000100;	// Interrupt when keys are pressed or released.
	SH7305_KEYSC.INTERRUPT.KYCPU_IE = 0b0000010;	// Falling + pressed + rising.
	SH7305_KEYSC.SCAN_WAIT.TIME = 0x05;		// No time between each interrupt.
	SH7305_KEYSC.SCAN_INTERVAL = 0x98;		// Unknown, use Casio's value. (scan interval)
	SH7305_KEYSC.KYOUTDR.KYO5DT = 0b11;		// Trigger when high-impedance state is detected.
	SH7305_KEYSC.KYOUTDR.KYO4DT = 0b11;		// Trigger when high-impedance state is detected.
	SH7305_KEYSC.KYOUTDR.KYO3DT = 0b11;		// Trigger when high-impedance state is detected.
	SH7305_KEYSC.KYOUTDR.KYO2DT = 0b11;		// Trigger when high-impedance state is detected.
	SH7305_KEYSC.KYOUTDR.KYO1DT = 0b11;		// Trigger when high-impedance state is detected.
	SH7305_KEYSC.KYOUTDR.KYO0DT = 0b11;		// Trigger when high-impedance state is detected.
	SH7305_KEYSC.KYINDR.KYDIR6 = 1;			// Scan [F1],[SHIFT],[ALPHA],[x,o,T],[div],[7],[4],[1],[0]
	SH7305_KEYSC.KYINDR.KYDIR5 = 1;			// Scan [F2],[OPT],[^2],[log],[FD],[8],[5],[2],[.]
	SH7305_KEYSC.KYINDR.KYDIR4 = 1;			// Scan [F3],[VAR],[^],[ln],[(],[9],[6],[3],[x10]
	SH7305_KEYSC.KYINDR.KYDIR3 = 1;			// Scan [F4],[MENU],[EXIT],[sin],[)],[DEL],[x],[+],[(-)]
	SH7305_KEYSC.KYINDR.KYDIR2 = 1;			// Scan [F5],[left],[down],[cos],[,],[%],[-],[EXE]
	SH7305_KEYSC.KYINDR.KYDIR1 = 1;			// Scan [F6],[up],[right],[tan],[->]
	SH7305_INTC.IPRF.KEYSC = 10;			// Set KEYSC interrupt priority (max).
	SH7305_INTC.IMCR5.KEYI = 1;			// Clear KEYSC interrupt mask.

	// Configure power / sleep mode.
	SH7305_POWER.STBCR.LONG_WORD = 0x00000000;
	//SH7305_POWER.STBCR.STBY = 1;
	//TODO: handle real-power on.
	
	// Configure TMU.
	SH7305_TMU.TSTR.BYTE = 0x00;			// Stop all timer
	for (int i = 0 ; i < 3 ; i = i + 1)
	{
		SH7305_TMU.TIMER[i].TCOR = 0xffffffff;		// Reset Timer
		SH7305_TMU.TIMER[i].TCOR = 0xffffffff;		// Reset Timer
		SH7305_TMU.TIMER[i].TCR.UNF  = 0;		// Clear interrupt flag.
		SH7305_TMU.TIMER[i].TCR.UNIE = 0;		// Disable interrupt.
		SH7305_TMU.TIMER[i].TCR.TPSC = 0b0000;		// Select clock deviser.
	}
	SH7305_INTC.IPRA.TMU0_0 = 12;
	SH7305_INTC.IPRA.TMU0_1 = 12;
	SH7305_INTC.IPRA.TMU0_2 = 12;
	SH7305_INTC.IMCR4.TUNI2 = 1;
	SH7305_INTC.IMCR4.TUNI1 = 1;
	SH7305_INTC.IMCR4.TUNI0 = 1;
}
