#include <kernel/process.h>
#include <lib/string.h>

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

	// Set process name.
	strncpy(process->name, name, PROCESS_NAME_LENGHT);

	// Initialize context.
	for (int i = 0 ; i < 16 ; i = i + 1)
		process->context.reg[i] = 0x00000000;
	process->context.gbr  = 0x00000000;
	process->context.macl = 0x00000000;
	process->context.mach = 0x00000000;
	process->context.ssr  = 0x00000000;
	process->context.spc  = 0x00000000;

	// Initialize processes.
	process->parent = process_current;
	process->child = NULL;
	process->next = NULL;
	return (process_pid);
}
