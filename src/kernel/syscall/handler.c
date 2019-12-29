#include <kernel/syscall.h>
#include <lib/display.h>

static const void *sys_handler[] = {
	NULL,		//restart
	NULL,		//exit
	sys_fork,	//fork
	NULL,		//read
	NULL,		//write
	NULL,		//open
	NULL,		//close
	sys_waitpid,	//waitpid
};

void *sys_get_handler(int sysno)
{
	//FIXME: Check sysno validity
	return ((void *)sys_handler[sysno]);
}
