#include <lib/stdio.h>
#include <lib/unistd.h>

int main(void)
{
	setpgid(0, 12);
	printf("%-6d%-6d%-6d\n", getppid(), getpid(), getpgid());
	printf("child proc fini...try to exit\n");
	for(int i = 0 ; i < 3000000 ; ++i);
	return (84);
}
