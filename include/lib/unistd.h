#ifndef __LIB_UNISTD_H__
# define __LIB_UNISTD_H__

#include <stddef.h>
#include <stdint.h>
#include <kernel/types.h>

// Define syscall LIST
#include <kernel/unistd_32.h>


//TODO: move me
#define WNOHANG		0
#define WUNTRACED	1
#define WCONTINUED	2
extern pid_t waitpid(pid_t pid, int *wstatus, int options);

// File syscall
#define O_DIRECT	0
extern int open(const char *pathname, int flags, ...);
extern ssize_t write(int fd, const void *buf, size_t count);
extern ssize_t read(int fd, void *buf, size_t count);
extern int close(int fd);

#endif /*__LIB_UNISTD_H__*/
