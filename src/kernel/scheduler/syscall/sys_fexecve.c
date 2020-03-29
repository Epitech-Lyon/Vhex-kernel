#include <kernel/process.h>
#include <kernel/scheduler.h>
#include <kernel/loader.h>
#include <kernel/util/atomic.h>
#include <kernel/devices/earlyterm.h>
#include <kernel/memory.h>
#include <string.h>

//TODO
//TODO COPY-ON-WRITE !!
//TODO
static void proc_dump_shared(struct process *child, struct process *parent)
{
	// Dump all opened file
	for (int i = 0 ; i < PROCESS_NB_OPEN_FILE ; ++i)
	{
		memcpy(&child->opfile[i].file, &parent->opfile[i].file, sizeof(FILE));
		child->opfile[i].status = parent->opfile[i].status;
	}

	// Dump specific
	memcpy(&child->tty, &parent->tty, sizeof(FILE));
}

static int generate_arguments(struct process *proc, char **argv, char **envp)
{
	// Generate argc
	proc->context.reg[4] = -1;
	while (argv[++proc->context.reg[4]] != NULL);

	// Generate process argv area
	proc->context.reg[5] = (uint32_t)pm_heap_alloc(&proc->memory.heap, sizeof(char*) * proc->context.reg[4]);
	if (proc->context.reg[5] == 0x00000000)
		return (-1);
	
	// Dump argv
	for (uint32_t i = 0 ; i < proc->context.reg[4] ; ++i)
	{
		((char**)proc->context.reg[5])[i] = pm_heap_alloc(&proc->memory.heap, strlen(argv[i]));
		strcpy(((char**)proc->context.reg[5])[i], argv[i]);
	}
	
	//TODO: envp
	(void)envp;
	return (0);
}

//TODO
//TODO Return EAGAIN if no proc can be created !!
//TODO Return ENOMEM if no memories can be allocated !!
//TODO
pid_t sys_fexecve(const char *pathname, char **argv, char **envp)
{
	extern struct process *process_current;
	struct process *proc;

	// CHeck error
	// TODO: set errno to EFAULT
	if (argv == NULL)
		return (-1);

	// Start atomic operation
	atomic_start();

	// Try create new process
	proc = process_create();
	if (proc == NULL)
	{
		earlyterm_write("sys_fexecve: process_create error !\n");
		DBG_WAIT;
		atomic_stop();
		return (-1);
	}

	// Dump parent process shared informations
	if (process_current != NULL)
		proc_dump_shared(proc, process_current);

	// Try to load binary into physical memory
	if (loader(proc, pathname) != 0)
	{
		earlyterm_write("sys_fexecve: loader error !");
		DBG_WAIT;
		process_free(proc);
		atomic_stop();
		return (-1);
	}

	// Generate arguments
	if (generate_arguments(proc, argv, envp) != 0)
	{
		earlyterm_write("sys_fexecve: arguements error\n");
		DBG_WAIT;
		process_free(proc);
		atomic_stop();
		return (-1);
	}

	// Release child process
	proc->sibling = process_current->child;
	process_current->child = proc;

	// Add new process into task queue
	if (sched_add_task(proc))
	{
		earlyterm_write("sys_fexecve: scheduler error !");
		DBG_WAIT;
		process_free(proc);
		atomic_stop();
		return (-1);
	}

	// Debug
	//earlyterm_write("New proc sched added !\n");
	///earlyterm_write("New proc PID = %#x !\n", proc->pid);
	//DBG_WAIT;

	// Stop atomic operations
	atomic_stop();
	return (proc->pid);
}
