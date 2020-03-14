#include <kernel/scheduler.h>
#include <kernel/devices/earlyterm.h>

void sys_exit(int status)
{
	// TODO handle status
	(void)status;

	// Fow now, just wait manual reset...
	earlyterm_clear();
	earlyterm_write("PROCESS EXIT SYSCALL !!!!");
	earlyterm_write("Wait manual reset...");
	while (1);
}
