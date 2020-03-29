#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

/*static int tree(const char *pathname)
{
	struct dirent dir;
	int ret;
	int fd;

	// Open directory
	fd = open(pathname, O_RDONLY | O_DIRECTORY);
	if (fd < 0) {
		dprintf(STDERR_FILENO, "tree: Cannot open directory '%s'\n", pathname);
		return (2);
	}

	ret = 0;
	while (1)
	{
		// Get dirent
		read = sys_getdents(fd, &dir, sizeof(struct dirent));
		if (read <= 0)
			return ((read == 0) ? 0 : 84);


	}

	return (0);
}*/

int main(int argc, char **argv)
{
	char c[1024];
	int ret;
	int fd;

	// Check parameters
	for (int i = 0 ; i < argc ; i++)
		printf("argv[%d]: %s\n", i, argv[i]);
	for(int i = 0 ; i < 3000000 ; ++i);

	// Read test
	fd = open("/mnt/casio/VHEX/text.txt", O_RDONLY);
	if (fd < 0) {
		dprintf(STDERR_FILENO, "unable to open test file\n");
		return (84);
	}

	while (1)
	{
		ret = read(fd, c, 1024);
		if (ret <= 0)
			break;
		write(STDOUT_FILENO, c, ret);
	}
	return (0);
}



//	setpgid(0, 12);
//	printf("%-6d%-6d%-6d\n", getppid(), getpid(), getpgid());
//	printf("child proc fini...try to exit\n");
//	for(int i = 0 ; i < 3000000 ; ++i);
//	return (84);
//}
