#include <kernel/process.h>
#include <kernel/unistd_32.h>
#include <kernel/memory.h>
#include <kernel/util.h>

pid_t process_create(const char *name)
{
	extern process_t *process_current;
	process_t *process;
	pid_t process_pid;

	// Check error
	if (name == NULL)
		return (-1);

	// Try to find free slot.
	process_pid = process_alloc(&process);
	if (process == NULL)
		return (-1);

	// Initialize stack
	process->context.reg[15] = (uint32_t)pm_alloc(PROCESS_STACK_SIZE);
	if (process->context.reg[15] == 0x00000000)
	{
		//TODO:  errno
		//FIXME: free allocated process.
		return (-1);
	}

	// Set process name.
	strncpy(process->name, name, PROCESS_NAME_LENGHT);

	// Initialize context.
	for (int i = 0 ; i < 15 ; i = i + 1)
		process->context.reg[i] = 0x00000000;

	// Initialize "special" registers.
	process->context.gbr  = 0x00000000;
	process->context.macl = 0x00000000;
	process->context.mach = 0x00000000;
	process->context.ssr  = 0x00000000;
	process->context.spc  = 0x00000000;

	// initialize "exit" part.
	uint8_t callexit[6] = {
		0b11000011, __NR_exit,	// trapa #__NR_exit
		0b10110000, 0b00000100,	// bsr PC + 2 - 4
		0b00000000, 0b00001001	// nop
	};
	process->context.pr = (uint32_t)pm_alloc(6);
	if (process->context.pr == 0x00000000)
	{
		//TODO: errno
		//FIXME: free allocated process.
		return (-1);
	}
	memcpy((void *)process->context.pr, callexit, 6);

	// Initialize processes.
	process->parent = process_current;
	process->child = NULL;
	process->next = NULL;
	return (process_pid);
}
