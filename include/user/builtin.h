#ifndef __USER_BUILTIN_H__
# define __USER_BUILTIN_H__

#include <stddef.h>
#include <stdint.h>

#define VHEX_BUILTIN(bname)				\
	static int bname(int argc, char **argv);	\
	__attribute__((section(".builtin")))		\
	struct builtin_s _##bname = {			\
		.name = #bname,				\
		.entry = &bname				\
	};						\
	static int bname(int argc, char **argv)

// Define builtin struct.
struct builtin_s
{
	char name[10];
	int (*entry)(int argc, char **argv);
};


#endif /*__USER_BUILTIN_H__*/
