#include <kernel/syscall.h>

pid_t sys_wait(int *wstatus)
{
	return (sys_waitpid(-1, wstatus, 0));
}
