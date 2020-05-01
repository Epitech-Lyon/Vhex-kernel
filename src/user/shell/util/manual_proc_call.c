#include "util.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

int manual_proc_call(char **argv)
{
	char c[1024];
	int ret;
	int fd;

	// Try to open test file
	(void)argv;
	fd = open("/mnt/casio/VHEX/text.txt", O_RDONLY);
	if (fd < 0) {
		dprintf(STDERR_FILENO, "unable to open test file\n");
		return (84);
	}

	// Display file
	while (1)
	{
		ret = read(fd, c, 1020);
		if (ret <= 0)
			break;
		write(STDOUT_FILENO, c, ret);
	}
	return (0);

//	char pathname[32];
//	pid_t child;

	// Generate pathname
	// TODO: handle PATH
	//strcpy(pathname, "/mnt/casio/VHEX/");
	//strcat(pathname, argv[0]);

	// Try to call binary from eeprom
//	printf("try to call '%s'\n", pathname);
//	for(int i = 0 ; i < 3000000 ; ++i);
//	child = fexecve(pathname, argv, NULL);
//	if (child != 0)
	//	return (-1);

	// Wait child process
//	wait_child(child);
//	return (0);
}
