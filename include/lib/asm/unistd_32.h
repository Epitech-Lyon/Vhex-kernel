#ifndef __ASM_UNISTD_32_H__
# define __ASM_UNISTD_32_H__

// Define the number of syscall
#define __NR_MAX		21

// Kernel Test
#define __NR_test_syscall	0

// Process
#define __NR_exit 		1
#define __NR_fexecve 		2
#define __NR_waitpid 		3
#define __NR_wait 		4
#define __NR_getpid 		5
#define __NR_getppid 		6
#define __NR_getpgid 		7
#define __NR_setpgid 		8

// VFS
#define __NR_read 		9
#define __NR_write		10
#define __NR_open 		11
#define __NR_close 		12
#define __NR_lseek 		13
#define __NR_pread 		14
#define __NR_pwrite		15

// Memory
#define __NR_mmap		16
#define __NR_munmap		17
#define __NR_proc_heap_alloc	18
#define __NR_proc_heap_free	19
#define __NR_proc_heap_realloc	20

#endif /*__ASM_UNISTD_32_H__*/
