#include <kernel/process.h>
#include <asm/unistd_32.h>
#include <kernel/memory.h>
#include <kernel/devices/earlyterm.h>
#include <lib/string.h>
#include <sys/signal.h>

/* proc_stacks_create() - create user and kernel stack */
static int proc_stacks_create(struct process *proc)
{
	// Initialize user stack
	proc->memory.stack.size.user = PROCESS_USER_STACK_SIZE;
	proc->memory.stack.user = pm_pages_alloc(PM_SIZE_TO_PAGES(proc->memory.stack.size.user));
	if (proc->memory.stack.user == NULL)
	{
		earlyterm_write("proc_error: user stack error !");
		DBG_WAIT;
		process_free(proc);
		return (-1);
	}
	proc->context.reg[15] = (uint32_t)proc->memory.stack.user + proc->memory.stack.size.user;

	// Initialize kernel stack
	proc->memory.stack.size.kernel = PROCESS_KERNEL_STACK_SIZE;
	proc->memory.stack.kernel = pm_pages_alloc(PM_SIZE_TO_PAGES(proc->memory.stack.size.kernel));
	if (proc->memory.stack.kernel == NULL)
	{
		earlyterm_write("proc_error: kernel stack error !");
		DBG_WAIT;
		pm_pages_free((void *)proc->memory.stack.user);
		process_free(proc);
		return (-1);
	}
	proc->stack.kernel = proc->memory.stack.kernel + proc->memory.stack.size.kernel;
	return (0);
}

/* proc_exit_inject() - inject "exit" procedure (involved at main()'s returning) */
static int proc_exit_inject(struct process *proc)
{
	uint8_t callexit[8] = {
		0b01100100, 0b00000011,	// mov r0, r4
		0b11000011, __NR_exit,	// trapa #__NR_exit
		0b10110000, 0b00000100,	// bsr PC + 2 - 4
		0b00000000, 0b00001001	// nop
	};

	// Inject exit procedure
	memcpy(proc->memory.stack.user, callexit, 8);
	proc->context.pr = (uint32_t)proc->memory.stack.user;
	return (0);
}

/* proc_context_init() - Initialise first program context */
static void proc_context_init(struct process *proc)
{
	// Initialize context.
	for (int i = 0 ; i < 15 ; i = i + 1)
		proc->context.reg[i] = 0x00000000;

	// Initialize "special" registers.
	proc->context.gbr  = 0x00000000;
	proc->context.macl = 0x00000000;
	proc->context.mach = 0x00000000;
	proc->context.ssr  = 0x40000000;	// <- force privilegied mode !
	proc->context.spc  = 0x00000000;
}

/* proc_file_init() - Initialize file part */
static void proc_file_init(struct process *proc)
{
	extern struct dentry *vfs_root_node;

	// Initialise file cache
	for (int i = 0 ; i < PROCESS_NB_OPEN_FILE ; i = i + 1)
	{
		proc->opfile[i].status = PROCESS_FILE_SLOT_UNUSED;
		proc->opfile[i].file.private = NULL;
		proc->opfile[i].file.file_op = NULL;
		proc->opfile[i].file.permission = 0;
		proc->opfile[i].file.cursor = 0;
	}

	// Initialize "special" file
	proc->working_dir = vfs_root_node;
	proc->tty.private = NULL;
}

/* proc_signal_init() - Initialize signal management */
static void proc_signal_init(struct process *proc)
{
	// Default value
	for (int i = 0 ; i < NSIG ; ++i)
		proc->signal[i] = __SIGUNDEF;

	// Init pending and blocked signals
	proc->sig_blocked = 0x00000000;
	proc->sig_pending = 0x00000000;

	// Initialize signals handled
	proc->signal[SIGSTOP] = SIG_DFL;
	proc->signal[SIGCONT] = SIG_DFL;
	proc->signal[SIGCHLD] = SIG_DFL;
	proc->signal[SIGTERM] = SIG_DFL;
	proc->signal[SIGKILL] = SIG_DFL;
	proc->signal[SIGSEGV] = SIG_DFL;
	proc->signal[SIGINT] = SIG_DFL;
	proc->signal[SIGHUP] = SIG_IGN;
	proc->signal[SIGFPE] = SIG_DFL;
}

struct process *process_create(void)
{
	extern struct process *process_current;
	struct process *process;

	// Try to initialise new process
	process = process_alloc();
	if (process == NULL
			|| proc_stacks_create(process) != 0
			|| proc_exit_inject(process) != 0) {
		earlyterm_write("proc_error: alloc error !");
		return (NULL);
	}

	// Initialise internal process part
	proc_context_init(process);
	proc_file_init(process);
	proc_signal_init(process);

	// Initialize heap
	process->memory.heap = NULL;

	// Initialize internal data
	process->sched_task = NULL;
	process->__stat_loc = 0x00000000;

	// Link new process with his parent.
	// @Note:
	// 	But de not link praent with new
	// process to avoid crash with sheduler.
	// The new process will be visible only
	// when it is registered by the sheduler.
	process->parent = process_current;
	process->sibling = NULL;
	process->child = NULL;
	return (process);
}
