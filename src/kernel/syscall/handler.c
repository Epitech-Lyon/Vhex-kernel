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

static const void *sys_handler[14] = {
	sys_test,	//restart
	NULL,		//exit
	NULL,		//exiec
	NULL,		//read
	NULL,		//write
	NULL,		//open
	NULL,		//close
	sys_waitpid,	//waitpid

	// Custom
	kvram_display,	//kvram_display
	kvram_clear,	//kvram_clear
	kvram_print,	//kvram_print
	kvram_ascii,	//kvram_ascii
	kvram_reverse,	//kvram_reverse
	kvram_scroll	//kvram_scroll
};

void *sys_get_handler(int sysno)
{
	// Check sysno validity
	if (sysno < 0 || sysno >= 14)
		return (0);

	// DEBUG
/*	kvram_clear();
	printk(0, 0, "sysno   = %d", sysno);
	printk(0, 0, "handler = %p", sys_handler[sysno]);
	kvram_display();
	DBG_WAIT;
*/
	// Return syscall 
	return ((void *)sys_handler[sysno]);
}
