#include <kernel/syscall.h>
#include <kernel/util.h>

static void sys_test(uint32_t a, uint32_t b, uint32_t c, uint32_t d)
{
	kvram_clear();
	printk(0, 0, "a = %#x", a);
	printk(0, 1, "b = %#x", b);
	printk(0, 2, "c = %#x", c);
	printk(0, 3, "d = %#x", d);
	kvram_display();
	DBG_WAIT;
}

static const void *sys_handler[16] = {
	// Kernel Test
	sys_test,		// test

	// Process
	sys_exit,		// exit
	sys_fexecve,		// fexecve
	sys_waitpid,		// waitpid

	// VFS
	sys_read,		// read
	sys_write,		// write
	sys_open,		// open
	sys_close,		// close
	sys_lseek,		// lseek

	// Display
	kvram_display,		// kvram_display
	kvram_clear,		// kvram_clear
	printk,			// printk
	kvram_ascii,		// kvram_ascii
	kvram_reverse,		// kvram_reverse
	kvram_scroll,		// kvram_scroll
	kvram_clr_str_area	// kvram_clr_str_area
};

void *sys_get_handler(int sysno)
{
	// Check sysno validity
	if (sysno < 0 || sysno >= 16)
		return (0);

	// DEBUG
	/*kvram_clear();
	printk(0, 0, "TRAPA ABI pre_handler !");
	printk(0, 1, "sysno   = %d", sysno);
	printk(0, 2, "handler = %p", sys_handler[sysno]);
	kvram_display();
	DBG_WAIT;
	DBG_WAIT;
	DBG_WAIT;*/

	// Return syscall 
	return ((void *)sys_handler[sysno]);
}
