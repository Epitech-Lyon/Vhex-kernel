/*#include "util.h"
#include "builtin.h"
#include <lib/string.h>

//TODO: use agc, argv.
int check_builtin(char *cmd)
{
	extern uint32_t bbuiltin_section;
	extern uint32_t ebuiltin_section;
	struct builtin_s *list;
	int wstatus;
	//pid_t pid;
	int i;

	i = -1;
	list = (void*)&bbuiltin_section;
	while ((uint32_t)&list[++i] < (uint32_t)&ebuiltin_section)
	{
		if (strcmp(list[i].name, cmd) != 0)
			continue;
			
		// Create subprocess
		//pid = fork();
		//if (pid < 0)
		//	return (1);
	
		// If we are the child execute
		// the builtins.
		//if (pid == 0)
		//{
			dclear();
			dprint(0, 0, "Child process !!");
			dprint(0, 1, "PID  = %d", getpid());
			dprint(0, 2, "PPID = %d", getppid());
			dupdate();

//			list[i].entry(0, NULL);
			return (0);
	//	} else {
	//		waitpid(pid, &wstatus, WCONTINUED);
			//TODO: signal handling.
	//	}
//	}
//	return (1);
}*/
