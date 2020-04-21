#include <kernel/signal.h>
#include <kernel/process.h>
#include <kernel/scheduler.h>
#include <kernel/util/atomic.h>
#include <kernel/devices/earlyterm.h>
#include <sys/signal.h>

int signal_raise(struct process *proc, int sig)
{
	// Check signal validity
	if (sig >= NSIG)
		return (-1);

	// Check if the signal is implemented
	if (proc->signal[sig] == __SIGUNDEF)
		return (-2);

	// Check initerruptible task
	if (proc->sched_task->status == SCHED_TASK_UNINTERRUPTIBLE)
		return (-3);

	// Start atomic operations
	atomic_start();

	// Check interruptible sleep
	if (proc->sched_task->status == SCHED_TASK_INTERRUPTIBLE)
		sched_task_wake_up(proc);

	// Check SIGCONT
	if (sig == SIGCONT && proc->sched_task->status == SCHED_TASK_STOPPED)
		sched_task_continue(proc);

	// Pend the signal
	proc->sig_pending |= sig;

	// Stop atomic operation and return
	atomic_stop();
	return (0);
}
