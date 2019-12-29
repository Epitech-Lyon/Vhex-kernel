#ifndef __LIB_UNISTD_H__
# define __LIB_UNISTD_H__

#include <stddef.h>
#include <stdint.h>
#include <kernel/types.h>

// Define syscall LIST
#include <kernel/unistd_32.h>

extern pid_t fork(void);

//TODO: move me
#define WNOHANG		0
#define WUNTRACED	1
#define WCONTINUED	2
extern pid_t waitpid(pid_t pid, int *wstatus, int options);

#endif /*__LIB_UNISTD_H__*/
