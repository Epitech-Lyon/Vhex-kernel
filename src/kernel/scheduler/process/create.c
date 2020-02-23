#include <kernel/process.h>
#include <kernel/util/unistd_32.h>
#include <kernel/memory.h>
#include <kernel/util/debug.h>
#include <kernel/util/string.h>

struct process *process_create(const char *name)
{
	extern struct process *process_current;
	extern struct dentry *vfs_root_node;
	struct process *process;
	pid_t process_pid;

	// Check error
	if (name == NULL)
		return (NULL);

	// Try to find free slot.
	process_pid = process_alloc(&process);
	if (process == NULL)
	{
		kvram_clear();
		printk(0, 0, "proc_error: alloc error !");
		kvram_display();
		DBG_WAIT;
		return (NULL);
	}

	// Initialize user stack
	process->memory.stack.size.user = PROCESS_USER_STACK_SIZE;
	process->memory.stack.user = (uint32_t)pm_alloc(process->memory.stack.size.user);
	if (process->memory.stack.user == 0x00000000)
	{
		kvram_clear();
		printk(0, 0, "proc_error: user stack error !");
		kvram_display();
		DBG_WAIT;
		process_free(process);
		return (NULL);
	}
	process->context.reg[15] = process->memory.stack.user + process->memory.stack.size.user;

	// Initialize kernel stack
	process->memory.stack.size.kernel = PROCESS_KERNEL_STACK_SIZE;
	process->memory.stack.kernel = (uint32_t)pm_alloc(process->memory.stack.size.kernel);
	if (process->memory.stack.kernel == 0x00000000)
	{
		kvram_clear();
		printk(0, 0, "proc_error: kernel stack error !");
		kvram_display();
		DBG_WAIT;
		pm_free((void *)process->memory.stack.user);
		process_free(process);
		return (NULL);
	}
	process->stack.kernel = process->memory.stack.kernel + process->memory.stack.size.kernel;

	// initialize "exit" part.
	uint8_t callexit[6] = {
		0b11000011, __NR_exit,	// trapa #__NR_exit
		0b10110000, 0b00000100,	// bsr PC + 2 - 4
		0b00000000, 0b00001001	// nop
	};
	process->memory.exit.size = 6;
	process->memory.exit.start = (uint32_t)(pm_alloc(process->memory.exit.size));
	if (process->memory.exit.start == 0x00000000)
	{
		pm_free((void *)process->memory.stack.user);
		pm_free((void *)process->memory.stack.kernel);
		process_free(process);
		return (NULL);
	}
	process->context.pr = process->memory.exit.start;
	memcpy((void *)process->memory.exit.start, callexit, 6);

	// Set process name.
	strncpy(process->name, name, PROCESS_NAME_LENGHT);

	// Initialize context.
	for (int i = 0 ; i < 15 ; i = i + 1)
		process->context.reg[i] = 0x00000000;

	// Initialize "special" registers.
	process->context.gbr  = 0x00000000;
	process->context.macl = 0x00000000;
	process->context.mach = 0x00000000;
	process->context.ssr  = 0x40000000;	// <- force privilegied mode !
	process->context.spc  = 0x00000000;

	// Initialise file cache
	for (int i = 0 ; i < PROCESS_NB_OPEN_FILE ; i = i + 1)
	{
		process->opfile[i].status = PROCESS_FILE_SLOT_UNUSED;
		process->opfile[i].file.private = NULL;
		process->opfile[i].file.file_op = NULL;
		process->opfile[i].file.permission = 0;
		process->opfile[i].file.cursor = 0;
	}
	process->working_dir = vfs_root_node;

	// DEBUG !
	kvram_clear();
	printk(0, 0, "proc_create: success !");
	printk(0, 1, "user stack:   %p", process->context.reg[15]);
	printk(0, 2, "kernel stack: %p", process->memory.stack.kernel);
	kvram_display();
	DBG_WAIT;

	// Link new process with his parent.
	// @Note:
	// 	But de not link praent with new
	// process to avoid crash with sheduler.
	// The new process will be visible only
	// when it is registered by the sheduler.
	process->parent = process_current;
	process->child = NULL;
	process->next = NULL;
	return (process);
}
