#include <kernel/devices/ubc.h>
#include <kernel/hardware/ubc.h>
#include <kernel/hardware/power.h>
#include <kernel/syscall.h>
#include <kernel/dbr.h>
#include <kernel/extra.h>
#include <lib/display.h>

// Internal data used by UBC device.
void *casio_dbr;

// Internal function.
extern void test(void);
extern void ubc_handler_pre(void);

int ubc_open(void)
{
	//FIXME: check MPU before call this function !!!
	// Power ON the User Break Controller.
	SH7305_POWER.MSTPCR0.UBC = 0;

	// Set Debug Based Register address.
	casio_dbr = dbr_set(&ubc_handler_pre);

	// Setup Channel 0.
	SH7305_UBC.CRR0.PCB	= 1;			// Set PC break adter instruction break.
	SH7305_UBC.CRR0.BIE	= 1;			// Request a Break.
	
	SH7305_UBC.CBR0.MFE	= 0;			// Enable Match Flag.
	SH7305_UBC.CBR0.MFI	= 0b000000;		// Set UBC.CCMFR.MF0 = 1, when break occur.
	SH7305_UBC.CBR0.AIE	= 0;			// Disable ASID check.
	SH7305_UBC.CBR0.SZ 	= 0b010;		// Disable Match condition.
	SH7305_UBC.CBR0.CD 	= 0;			// Use Operand Bus for Operand Access.
	SH7305_UBC.CBR0.ID	= 0b01;			// Selecte instruction Fetch cycle.
	SH7305_UBC.CBR0.RW	= 0b11;			// Use Read or Write for match condition.
	SH7305_UBC.CBR0.CE	= 0;			// Disable Channel 0.
	
	// Set up target address.
	SH7305_UBC.CAR0		= (uint32_t)&test;	// Tested programe address !
	SH7305_UBC.CAMR0	= 0x00000000;		// Address Mask.
	
	// Setup Control register.
	SH7305_UBC.CBCR.UBDE	= 1;			// Use DBR instead of VBR.


	//@note:
	//	You *SHOULD* use `icbi` SH4 instruction
	//	After channel enable, otherwise the calculator
	//	will freeze.
	SH7305_UBC.CBR0.CE	= 1;			// Enable Channel 0 !
	icbi((void*)0xa0000000);
	return (0);
}
