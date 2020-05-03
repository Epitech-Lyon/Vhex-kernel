#include <kernel/syscall.h>
#include <kernel/devices/earlyterm.h>
#include <asm/unistd_32.h>

static void sys_test(uint32_t a, uint32_t b, uint32_t c, uint32_t d)
{
	earlyterm_clear();
	earlyterm_write("Syscall test entry:\n");
	earlyterm_write("* a = %#x\n", a);
	earlyterm_write("* b = %#x\n", b);
	earlyterm_write("* c = %#x\n", c);
	earlyterm_write("* d = %#x\n", d);
	while (1);
}

static const void *sys_handler[__NR_MAX] = {
	// Kernel Test
	sys_test,		// test

	// Process
	sys_exit,		// exit
	sys_fexecve,		// fexecve
	sys_waitpid,		// waitpid
	sys_wait,		// wait
	sys_getpid,		// getpid
	sys_getppid,		// getppid
	sys_getpgid,		// getpgid
	sys_setpgid,		// setpgid

	// VFS
	sys_read,		// read
	sys_write,		// write
	sys_open,		// open
	sys_close,		// close
	sys_lseek,		// lseek
	sys_pread,		// pread
	sys_pwrite,		// pwrite

	// Memory
	sys_mmap,		// mmap
	NULL,			// munmap
	sys_proc_heap_alloc,	// (custom) process heap alloc
	sys_proc_heap_free,	// (custom) process heap free
	sys_proc_heap_realloc	// (custom) process heap realloc
};

void *sys_get_handler(int sysno)
{
	// Check sysno validity
	if (sysno < 0 || sysno >= __NR_MAX)
		return (NULL);

	// DEBUG
	//earlyterm_write("syscall (%d)!!\n", sysno);
	//DBG_WAIT;
	//DBG_WAIT;

	// Return syscall 
	return ((void *)sys_handler[sysno]);
}
