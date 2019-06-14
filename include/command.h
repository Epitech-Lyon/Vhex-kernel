#ifndef __COMMANDS_H__
# define __COMMANDS_H__

#include <stddef.h>
#include <stdint.h>
/* Define session_s */
#include <utils.h>

/* cmd_block - data part of command block */
struct cmd_block
{
	char *name;
	void (*constructor)(int, char **, struct session_s *, char *);
	const char *man;
};

// CMDBLOCK()
// Generate a "command block" stored in ".cmd.cahce" section.
// We need to "hide" the block, so we generate anonyme name using
// __COUNTER__ macros.
#define gen_name(n)	_##n
#define anonym_name(n)	gen_name(n)
#define CMDBLOCK(cmd,callback,string)					\
	__attribute__((section(".cmd.cache")))				\
	static const struct cmd_block anonym_name(__COUNTER__) = {	\
		.name = cmd,						\
		.constructor = callback,				\
		.man = string						\
	}

/* command_entry() - parse and execute the user's command. */
void command_entry(struct session_s *session, struct vhex_s *vhex);

/* cmd_cache_find() - find command in the cache section. */
const struct cmd_info *cmd_cache_find(char const *name);

#endif /*__COMMANDS_H__*/
