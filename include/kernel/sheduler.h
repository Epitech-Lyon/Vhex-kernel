#ifndef __KERNEL_SHEDULER_H__
# define __KERNEL_SHEDULER_H__

#include <stddef.h>
#include <stdint.h>

// Internal struct used by the sheduler
struct shed_task
{
	// Task status
	enum {
		RUNNING,
		SLEEPING,
		DOWN,
		STOPPED,
		ZOMBIE
	} status;

	// Process informations
	struct process_s process;

	// Preemptif part
	int8_t priority;
};

//---
// Internal function
//---

/*
** Add new process to sheduler
*/
extern int shed_add_task(struct process_s *process);

/*
** Start shedluler (in theory, this function is called only
** time by the kernel during "bootstrap" part).
*/
extern void shed_start(void);

/*
** Save current context of the current task and run the
** next one based on internal task priority.
*/
extern void shed_shedule(void);

#endif /*__KERNEL_SHEDULER_H__*/
