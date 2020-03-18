#ifndef __USER_BUILTIN_H__
# define __USER_BUILTIN_H__

#include <stddef.h>
#include <stdint.h>

// CMDBLOCK()
// Generate a "command block" stored in ".cmd.cahce" section.
// We need to "hide" the block, so we generate anonyme name using
// __COUNTER__ macros.
#define gen_name(n)	_##n
#define anonym_name(n)	gen_name(n)
#define VHEX_BUILTIN(builtin)						\
	static int builtin(int argc, char **argv);			\
	__attribute__((section(".builtin.cache")))			\
	static const struct builtin_s anonym_name(__COUNTER__) = {	\
		.name = #builtin,					\
		.entry = builtin					\
	};								\
	static int builtin(int argc, char **argv)

// Define builtin struct.
struct builtin_s
{
	char name[10];
	int (*entry)(int argc, char **argv);
};

// Builtin list
//extern int builtin_proc(void);
//extern int builtin_ram(void);


#endif /*__USER_BUILTIN_H__*/
