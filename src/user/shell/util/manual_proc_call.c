#include "util.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int manual_proc_call(char **argv)
{
	char pathname[32];
	int wstatus;
	pid_t child;

	printf("try to call '%s'\n", argv[0]);
	for(int i = 0 ; i < 30000000 ; ++i);

	// Generate pathname
	// TODO: handle PATH
	strcpy(pathname, "/mnt/casio/VHEX/");
	strcat(pathname, argv[0]);

	// Try to call binary from eeprom
	printf("try to call '%s'\n", pathname);
	for(int i = 0 ; i < 30000000 ; ++i);
	child = fexecve(pathname, argv, NULL);
	if (child != 0)
		return (-1);

	waitpid(child, &wstatus, 0);
	return (0);
}
