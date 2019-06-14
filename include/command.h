#ifndef __COMMANDS_H__
# define __COMMANDS_H__

#include <stddef.h>
#include <stdint.h>
#include <utils.h>

struct cmd_info
{
	char *name;
	void (*constructor)(int, char **, struct session_s *, char *);
	const char *man;
};


#define gen_name(n)	_##n
#define anonym_name(n)	gen_name(n)
#define CMDBLOCK(cmd,callback,string)					\
	__attribute__((section(".cmd.cache")))				\
	static const struct cmd_info anonym_name(__COUNTER__) = {	\
		.name = cmd,						\
		.constructor = callback,				\
		.man = string						\
	}

//TODO: write doc.
void command_entry(struct session_s *session, struct vhex_s *vhex);
const struct cmd_info *cmd_cache_find(char const *name);
/*void address_jump(int argc, char **argv, struct session_s *session, char *info);
void syscall_jump(int argc, char **argv, struct session_s *session, char *info);
void ram_jump(int argc, char **argv, struct session_s *session, char *info);
void vbr_jump(int argc, char **argv, struct session_s *session, char *info);
void quit_command(int argc, char **argv, struct session_s *session, char *info);
void where_command(int argc, char **argv, struct session_s *session, char *info);
void help_command(int argc, char **argv, struct session_s *session, char *info);
*/
#endif /*__COMMANDS_H__*/
