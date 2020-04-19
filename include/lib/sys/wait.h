#ifndef __LIB_SYS_WAIT_H__
# define __LIB_SYS_WAIT_H__

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

// Wait options
#define WNOHANG		0
#define WUNTRACED	1
#define WCONTINUED	2

// Signals
#define	__W_EXITCODE(ret, sig)	((ret) << 8 | (sig))
#define	__WEXITSTATUS(status)	(((status) & 0xff00) >> 8)
#define WEXITSTATUS(status)	__WEXITSTATUS(status)

// Wait process
extern pid_t waitpid(pid_t pid, int *wstatus, int options);
extern pid_t wait(int *wstatus);

#endif /*__LIB_SYS_WAIT_H__*/
