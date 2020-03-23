#ifndef __LIB_UNISTD_H__
# define __LIB_UNISTD_H__

#include <stddef.h>
#include <stdint.h>
#include <kernel/util/types.h>

// Define syscall LIST
#include <kernel/util/unistd_32.h>

// TODO: move me
#define STDOUT_FILENO	0
#define STDERR_FILENO	1
#define STDIN_FILENO	2

//TODO: move me
#define WNOHANG		0
#define WUNTRACED	1
#define WCONTINUED	2

//TODO: move me
#define	__W_EXITCODE(ret, sig)	((ret) << 8 | (sig))
#define	__WEXITSTATUS(status)	(((status) & 0xff00) >> 8)
#define WEXITSTATUS(status)	__WEXITSTATUS(status)

extern pid_t getpid(void);
extern pid_t getpgid(void);
extern pid_t getppid(void);
extern int setpgid(pid_t pid, pid_t pgid);
extern pid_t waitpid(pid_t pid, int *wstatus, int options);
extern pid_t fexecve(const char *pathname);

// File syscall
#define O_DIRECT	0
extern int open(const char *pathname, int flags, ...);
extern ssize_t write(int fd, const void *buf, size_t count);
extern ssize_t read(int fd, void *buf, size_t count);
extern int close(int fd);

#endif /*__LIB_UNISTD_H__*/
