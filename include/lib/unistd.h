#ifndef __LIB_UNISTD_H__
# define __LIB_UNISTD_H__

#include <stddef.h>
#include <stdint.h>
#include <kernel/util/types.h>

// Define syscall LIST
#include <kernel/util/unistd_32.h>

// TODO: move me
#define STDIN_FILENO	0
#define STDOUT_FILENO	1
#define STDERR_FILENO	2

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
// TODO: move me 
// FIXME: find real value
#ifndef __FILE_H__
# define O_DIRECT	0
# define O_DIRECTORY	1
# define O_RDONLY	2
# define O_WRONLY	4
# define O_RDWR 	8

# define SEEK_SET	0
# define SEEK_CUR	1
# define SEEK_END	2
#endif
extern int open(const char *pathname, int flags, ...);
extern ssize_t write(int fd, const void *buf, size_t count);
extern ssize_t read(int fd, void *buf, size_t count);
extern off_t lseek(int fd, off_t offset, int whence);
extern int close(int fd);

#endif /*__LIB_UNISTD_H__*/
