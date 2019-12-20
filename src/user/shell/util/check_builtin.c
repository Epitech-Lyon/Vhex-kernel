#include "util.h"
#include "builtin.h"
#include <lib/string.h>
#include <lib/display.h>

//TODO: use agc, argv.
int check_builtin(char *cmd)
{
	extern uint32_t bbuiltin_section;
	extern uint32_t ebuiltin_section;
	struct builtin_s *list;
	int i;

	i = -1;
	list = (void*)&bbuiltin_section;
	while ((uint32_t)&list[++i] < (uint32_t)&ebuiltin_section)
	{
		dclear();
		dprint(0, 0, "builtin - %s", list[i].name);
		dupdate();
		for (int i = 0 ; i < 9000000 ; i = i + 1);


		if (strcmp(list[i].name, cmd) == 0)
		{
			list[i].entry(0, NULL);
			return (0);
		}
	}
	return (1);
}
