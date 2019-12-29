#ifndef __KERNEL_PROCESS_H__
# define __KERNEL_PROCESS_H__

#include <stddef.h>
#include <stdint.h>
#include <kernel/context.h>
#include <kernel/types.h>

#define PROCESS_NAME_LENGHT	(16)
#define PROCESS_MAX		(3)

#define PROC_IDLE		(0)

// define process struct.
//TODO: signal !
typedef struct process_s
{
	// Process name.
	char name[PROCESS_NAME_LENGHT];

	// Context management
	common_context_t context;

	// Signals management.
	//sighandler_t signal[NSIG];
	
	// Other process management.
	struct process_s *parent;
	struct process_s *child;
	struct process_s *next;
} process_t;

// Internal struct used by the
// static process stack
struct process_stack_s
{
	struct process_s process;
	int status;
};

// Functions.
extern pid_t process_create(const char *name);
extern process_t *process_get(pid_t pid);
extern int process_switch(pid_t pid);

// Internal function.
extern pid_t process_alloc(process_t **process);

#endif /*__KERNEL_PROCESS_H__*/