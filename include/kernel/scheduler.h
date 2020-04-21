#ifndef __KERNEL_SHEDULER_H__
# define __KERNEL_SHEDULER_H__

#include <stddef.h>
#include <stdint.h>
#include <kernel/process.h>

// Define the number of task
#define SCHED_TASK_NB_MAX	PROCESS_MAX
#define SCHED_QUANTUM_TICKS	(4)

// Internal struct used by the sheduler
struct sched_task
{
	// Process informations
	struct process *process;

	// Internal scheduler part
	// @note: defined here to facilite the
	// management in assembly
	struct sched_task *next;


	// Task status
	enum {
		SCHED_TASK_UNUSED,
		SCHED_TASK_RUNNING,
		SCHED_TASK_ZOMBIE,
		SCHED_TASK_INTERRUPTIBLE,
		SCHED_TASK_UNINTERRUPTIBLE,
		SCHED_TASK_STOPPED,
	} status;

	// Preemptif part (not implemented yet)
	struct {
		uint8_t _static;
		int8_t _dynamic;
	} priority;
};

//---
// Internal function
//---
/*
** Initialize scheduler cache
** @note: only called once by the kernel.
*/
extern void sched_initialize(void);

/*
** Add new process to sheduler
*/
extern int sched_task_add(struct process *process);

/*
** Internal function wich will stop process (SIGSTOP)
*/
extern void sched_task_stop(struct process *process);

/*
** Internal function wich will allow process to "restart".
** (SIGCONT, continue signal)
*/
extern void sched_task_continue(struct process *process);

/*
** Wake up sleeping process if it's can be interrupted
*/
extern void sched_task_wake_up(struct process *process);

/*
** Start shedluler (in theory, this function is called only
** time by the kernel during "bootstrap" part).
*/
extern void sched_start(void);

/*
** Get the current task context and the next one
** based on internal task priority / status.
*/
extern int sched_schedule(common_context_t **current, common_context_t **next);

/*
** Internal function wich will switch current process
** context with the next context
*/
extern void sched_context_switch(common_context_t *current, common_context_t *next);

#endif /*__KERNEL_SHEDULER_H__*/
