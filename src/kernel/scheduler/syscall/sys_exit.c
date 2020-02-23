#include <kernel/scheduler.h>
#include <kernel/util/debug.h>

void sys_exit(int status)
{
	kvram_clear();
	printk(0, 0, "PROCESS EXIT SYSCALL !!!!");
	printk(0, 0, "Wait manual reset...");
	kvram_display();
	while (1);
}
