#include "util.h"
#include "builtin.h"
#include <string.h>
#include <unistd.h>

// Internal builtin list
// FIXME: due to PIE binary format, we can not
// FIXME: use address of builtin_proc !!
struct builtin_s builtin[2] = {
	{
		.name = "proc",
		.entry = (void*)&builtin_proc
	},
	{
		.name = "ram",
		.entry = NULL
	}
};



//TODO: use agc, argv.
int check_builtin(int argc, char **argv)
{
	for (int i = 0 ; i < 2 ; ++i)
	{
		if (strcmp(builtin[i].name, argv[0]) != 0)
			continue;
		if (builtin[i].entry != NULL)
			(*builtin[i].entry)(argc, argv);
		return (0);
	}
	return (-1);
}
