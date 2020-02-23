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
	// Used when interrupt or exception occur
	struct {
		uint32_t kernel;
		uint32_t user;
	} stack;

	// Context management
	common_context_t context;

	// Process name.
	char name[PROCESS_NAME_LENGHT];

	// Open file management
	struct {
		enum {
			PROCESS_FILE_SLOT_UNUSED,
			PROCESS_FILE_SLOT_USED
		} status;
		FILE file;
	} opfile[PROCESS_NB_OPEN_FILE];
	struct dentry *working_dir;

	// ignals management.
	//sighandler_t signal[NSIG];
	
	// Virtual / Physical memory management.
	// @note
	// 	For now, we can not use the MMU
	// so we just save all physical allocated
	// space. This is an hardcode of each
	// process memory management.
	struct {
		struct {
			uint32_t user;
			uint32_t kernel;
			struct {
				uint32_t user;
				uint32_t kernel;
			} size;
		} stack;
		struct {
			uint32_t start;
			uint32_t size;
		} program;
		struct {
			uint32_t start;
			uint32_t size;
		} exit;
	} memory; 
	
	// Other process management.
	struct process *parent;
	struct process *child;
	struct process *next;
};

// Internal struct used by the
// static process stack
struct process_stack
{
	// Indicate process slot status
	enum {
		PROC_USED,
		PROC_UNUSED
	} status;

	// Internal process data
	struct process process;
};

// Functions.
extern struct process *process_create(const char *name);
extern struct process *process_get(pid_t pid);
extern pid_t process_get_pid(struct process *process);
extern int process_switch(pid_t pid);

// Internal function.
extern pid_t process_alloc(struct process **process);
extern int process_free(struct process *process);

#endif /*__KERNEL_PROCESS_H__*/
