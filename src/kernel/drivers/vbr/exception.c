#include <kernel/devices/earlyterm.h>
#include <kernel/drivers/vbr.h>
#include <kernel/process.h>

void exception_handler(void)
{
	extern struct process *process_current;
	uint32_t stack;
	uint32_t spc;
	uint32_t ssr;
	uint32_t evt;
	uint32_t tea;

	// Get some registers's data.
	__asm__ volatile ("stc	spc, %0" : "=r"(spc));
	__asm__ volatile ("stc	ssr, %0" : "=r"(ssr));
	__asm__ volatile ("mov	r15, %0" : "=r"(stack));

	// Get event code and tea regsiter
	evt = *((uint32_t *)0xff000024);
	tea = *((uint32_t *)0xff00000c);

	// Display error screen
	earlyterm_clear();
	switch (evt)
	{
	case EXP_CODE_ACCESS_READ:
	case EXP_CODE_ACCESS_WRITE:
		earlyterm_write("CPU Access Violation (R/W)\n");
		earlyterm_write("> Address : %p\n", tea);
		break;

	case EXP_CODE_BAD_INSTR:
	case EXP_CODE_BAD_SLOTINSTR:
		if(evt == EXP_CODE_BAD_SLOTINSTR)
			earlyterm_write("Illegal slot instruction\n");
		else
			earlyterm_write("Illegal instruction\n");
		earlyterm_write("> TEA value = %p\n", tea);
		earlyterm_write("> TEA correct = %p\n", tea >> 2 << 2);
		break;

	case EXP_CODE_USER_BREAK:
		earlyterm_write("Unexpected User Break exception.\n");
		while(1);

	case EXP_CODE_TLB_INITWRITE:
		earlyterm_write("Initial MMU page write (bloking).\n");
		while (1);

	case EXP_CODE_TLB_PROTECT_R:
	case EXP_CODE_TLB_PROTECT_W:
		earlyterm_write("TLB protection violation (blocking)");
		while (1);

	case EXP_CODE_TLB_READ:
	case EXP_CODE_TLB_WRITE:
		earlyterm_write("TLB miss (blocking)");
		while (1);

	default:
		earlyterm_write("Ho crap ! Unkonow exception !\n");
		earlyterm_write("> EXPEVT : %#x\n", evt);
		earlyterm_write("> TEA value = %p\n", tea);

	}

	// Common display
	earlyterm_write("> SPC Value = %p\n", spc);
	earlyterm_write("> Process : %p\n", process_current);
	earlyterm_write("> Process entry : %p\n", process_current->memory.program.start);
	earlyterm_write("> Proc rela. PC = %p\n", spc - (uint32_t)process_current->memory.program.start);
	while (1);

/*	// Write exception informations.
	earlyterm_write(
		"Ho crap ! Exception !\n"
		"tra:    %#x\n"
		"expevt: %#x\n"
		"spc:    %#x\n"
		"ssr:    %#x\n"
		"sr:     %#x",
		*((uint32_t *)0xff000020),
		,
		spc,
		ssr,
		sr
	);
	while (1);*/
}
