#include <kernel/drivers/keyboard.h>
#include <kernel/drivers/vbr.h>
#include <kernel/hardware/keysc.h>
#include <kernel/hardware/intc.h>
#include <kernel/bits/driver.h>

// Internal module functions
extern void sh7305_keysc_int_handler(void);

// Internal hardware context
struct __sh7305_keysc_context keysc_context;

// Configure Key Scan (KEYSC)
static void sh7305_keysc_driver_install(void)
{
	// Mask interrupt (requered)
	SH7305_INTC.IMR5.KEYI = 1;			// Mask KEYSC interrupt.
	SH7305_INTC.IPRF.KEYSC = 0;			// Disable KEYSC interrupt.

	// Save current keysc context
	keysc_context.control		= SH7305_KEYSC.CONTROL.WORD;
	keysc_context.autofix		= SH7305_KEYSC.AUTOFIX.WORD;
	keysc_context.scan_mode		= SH7305_KEYSC.SCAN_MODE.WORD;
	keysc_context.scan_state	= SH7305_KEYSC.SCAN_STATE.WORD;
	keysc_context.interrupt		= SH7305_KEYSC.INTERRUPT.WORD;
	keysc_context.scan_wait		= SH7305_KEYSC.SCAN_WAIT.WORD;
	keysc_context.scan_interval	= SH7305_KEYSC.SCAN_INTERVAL;
	keysc_context.kyoutdr		= SH7305_KEYSC.KYOUTDR.WORD;
	keysc_context.kyindr		= SH7305_KEYSC.KYINDR.WORD;

	// Intialize key scan interface
	SH7305_KEYSC.CONTROL.KEYIE = 1;			// Enable KEYSC interrupt.
	SH7305_KEYSC.CONTROL.INTMODE = 0b00;		// Use default (?) interrupt mode.
	SH7305_KEYSC.AUTOFIX.ENABLE = 1;		// Enable key bounce fix (?)
	SH7305_KEYSC.AUTOFIX.UNKNOWN0 = 0b100;		// Unknown, use Casio's value. (autofix)
	SH7305_KEYSC.AUTOFIX.UNKNOWN1 = 0b10;		// Unknown, use Casio's value. (autofix)
	SH7305_KEYSC.SCAN_MODE.UNKNOWN = 0b001;		// Unknown, use Casio's value. (scan mode)
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

	// Install the interrupt handler
	vbr_interrupt_set(INT_CODE_KEYSC, &sh7305_keysc_int_handler);

	// Initialize high level abstractions
	keycache_initialize();

	// Enable interrupt
	SH7305_INTC.IPRF.KEYSC = 10;			// Set KEYSC interrupt priority (max).
	SH7305_INTC.IMCR5.KEYI = 1;			// Clear KEYSC interrupt mask.
}

// Restore old context
// @note: the interrupt priority will be restored by the INTC driver
static void sh7305_keysc_driver_uninstall(void)
{
	// Mask interrupt (requered)
	SH7305_INTC.IMR5.KEYI = 1;
	SH7305_INTC.IPRF.KEYSC = 0;

	// Restore keysc context
	SH7305_KEYSC.CONTROL.WORD = keysc_context.control;
	SH7305_KEYSC.AUTOFIX.WORD = keysc_context.autofix;
	SH7305_KEYSC.SCAN_MODE.WORD = keysc_context.scan_mode;
	SH7305_KEYSC.SCAN_STATE.WORD = keysc_context.scan_state;
	SH7305_KEYSC.INTERRUPT.WORD = keysc_context.interrupt;
	SH7305_KEYSC.SCAN_WAIT.WORD = keysc_context.scan_wait;
	SH7305_KEYSC.SCAN_INTERVAL = keysc_context.scan_interval;
	SH7305_KEYSC.KYOUTDR.WORD = keysc_context.kyoutdr;
	SH7305_KEYSC.KYINDR.WORD = keysc_context.kyindr;
}

// Create driver object
VHEX_DRIVER(4, driver_keyboard_sh7305) = {
	.arch = MPU_SH7305,
	.install = &sh7305_keysc_driver_install,
	.uninstall = &sh7305_keysc_driver_uninstall,
	.name = "keysc"
};
