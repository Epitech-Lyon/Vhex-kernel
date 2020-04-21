#include "util.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int manual_proc_call(char **argv)
{
	char pathname[32];
	pid_t child;

	// Generate pathname
	// TODO: handle PATH
	strcpy(pathname, "/mnt/casio/VHEX/");
	strcat(pathname, argv[0]);

	// Try to call binary from eeprom
	printf("try to call '%s'\n", pathname);
	for(int i = 0 ; i < 3000000 ; ++i);
	child = fexecve(pathname, argv, NULL);
	if (child != 0)
		return (-1);

	// Wait child process
	wait_child(child);
	return (0);
}
