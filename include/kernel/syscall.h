#ifndef __SYSCALL_H__
# define __SYSCALL_H__

#include <stdint.h>
#include <stddef.h>
#include <asm/types.h>

// Process
extern pid_t sys_getpid(void);
extern pid_t sys_getppid(void);
extern pid_t sys_getpgid(void);
extern pid_t sys_wait(int *wstatus);
extern int sys_setpgid(pid_t pid, pid_t pgid);
extern pid_t sys_waitpid(pid_t pid, int *wstatus, int options);
extern pid_t sys_fexecve(const char *pathname, char **argv, char **envp);
extern void sys_exit(int status);

// File
extern int sys_open(const char *pathname, int flags, ...);
extern ssize_t sys_write(int fd, const void *buf, size_t count);
extern ssize_t sys_read(int fd, void *buf, size_t count);
extern off_t sys_lseek(int fd, off_t offset, int whence);
extern ssize_t sys_pwrite(int fd, const void *buf, size_t count, off_t offset);
extern ssize_t sys_pread(int fd, void *buf, size_t count, off_t offset);
extern int sys_close(int fd);

// Memory
extern void *sys_mmap(void *addr, size_t length,
			int prot, int flags, int fd, off_t offset);
extern int sys_munmap(void *addr, size_t length);
extern void *sys_proc_heap_alloc(size_t size);
extern void *sys_proc_heap_realloc(void *ptr, size_t size);
extern void sys_proc_heap_free(void *ptr);

#endif /*__SYSCALL_H__*/
