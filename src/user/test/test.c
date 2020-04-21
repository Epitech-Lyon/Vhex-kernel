#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	char c[1024];
	int ret;
	int fd;

	// Check parameters
	for (int i = 0 ; i < argc ; i++)
		printf("argv[%d]: %s\n", i, argv[i]);
	for(int i = 0 ; i < 1000000 ; ++i);

	// Read test
	fd = open("/mnt/casio/VHEX/text.txt", O_RDONLY);
	if (fd < 0) {
		dprintf(STDERR_FILENO, "unable to open test file\n");
		return (84);
	}

	// DIsplay file
	while (1)
	{
		ret = read(fd, c, 1024);
		if (ret <= 0)
			break;
		write(STDOUT_FILENO, c, ret);
	}

	// Wait signal
	// TODO: better signal management (kernel)
	printf("wait signal....\n");
	while (read(STDIN_FILENO, c, 3) != 4);
	return (0);
}
