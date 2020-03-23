#ifndef __KERNEL_PROCESS_H__
# define __KERNEL_PROCESS_H__

#include <stddef.h>
#include <stdint.h>
#include <kernel/fs/file.h>
#include <kernel/fs/filesystem.h>
#include <kernel/context.h>
#include <kernel/util/types.h>

#define PROCESS_NB_OPEN_FILE		(4)
#define PROCESS_USER_STACK_SIZE		(2 * 1024)
#define PROCESS_KERNEL_STACK_SIZE	(512)
#define PROCESS_NAME_LENGHT		(16)
#define PROCESS_MAX			(4)

// define process struct.
//TODO: signal !
struct process
{
	//---
	// Used when interrupt or exception occur
	//---
	struct {
		void *kernel;
		void *user;
	} stack;


	//---
	// Context management
	//---
	common_context_t context;


	//---
	// Shared (child / parent) data informations
	//---
	struct {
		enum {
			PROCESS_FILE_SLOT_UNUSED,
			PROCESS_FILE_SLOT_USED
		} status;
		FILE file;
	} opfile[PROCESS_NB_OPEN_FILE];
	struct dentry *working_dir;
	FILE tty;


	//---
	// Signals management.
	//---
	//sighandler_t signal[NSIG];


	//---
	// Virtual / Physical memory management.
	//
	// @note
	// For now, we can not use the MMU so we just
	// save all physical allocated space. This is an
	// hardcode of each process memory management.
	//---
	struct {
		struct {
			void *user;
			void *kernel;
			struct {
				size_t user;
				size_t kernel;
			} size;
		} stack;
		struct {
			void *start;
			size_t size;
		} program;
		struct {
			void *start;
			size_t size;
		} exit;
	} memory; 


	//---
	// Other process management.
	//---
	struct process *parent;
	struct process *sibling;
	struct process *child;


	//---
	// Private data
	//---
	// Process ID
	pid_t pid;
	pid_t pgid;

	// Indicate process status
	enum {
		PROC_INIT,
		PROC_RUNNING,
		PROC_SLEEP,
		PROC_STOP,
		PROC_ZOMBIE,
		PROC_DEAD
	} status;

	// Used to by the _exit() syscall
	uint32_t __stat_loc;

	// Used by the allocator
	struct process *next;
};


// Functions.
extern struct process *process_create(void);
extern int process_switch(pid_t pid);


// Internals
extern struct process *process_alloc(void);
extern struct process *process_get(pid_t pid);
extern int process_free(struct process *process);

#endif /*__KERNEL_PROCESS_H__*/
