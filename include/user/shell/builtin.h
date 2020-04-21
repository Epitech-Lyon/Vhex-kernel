#ifndef __USER_BUILTIN_H__
# define __USER_BUILTIN_H__

#include <stddef.h>
#include <stdint.h>

// Define builtin struct.
struct builtin_s
{
	char name[10];
	int (*entry)(int argc, char **argv);
};

// Builtin list
extern int builtin_proc(int argc, char **argv);
extern int builtin_ram(void);
extern int builtin_fxdb(int argc, char **argv);


#endif /*__USER_BUILTIN_H__*/
