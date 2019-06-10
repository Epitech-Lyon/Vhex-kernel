#ifndef __COMMANDS_H__
# define __COMMANDS_H__

#include <stddef.h>
#include <stdint.h>
#include <utils.h>

//FIXME: MOVE ME !
struct cmd_info
{
	char *name;
	struct {
		ptrdiff_t anchor;
		size_t size;
	} update;
	void (*init)(int argc, char **argv, struct session_s *session, char *info);
};

#define CMDBLOCK(bname,cmd,ptr,sizez,callback)	\
	static const struct cmd_info bname = {		\
		.name = cmd,				\
		.update = {				\
			.anchor = (ptrdiff_t)(ptr),	\
			.size = sizez,			\
		},					\
		.init = callback			\
	}

//TODO: write doc.
void command_entry(struct session_s *session, struct dump_s *dump);
void address_jump(int argc, char **argv, struct session_s *session, char *info);
void syscall_jump(int argc, char **argv, struct session_s *session, char *info);
void ram_jump(int argc, char **argv, struct session_s *session, char *info);
void vbr_jump(int arc, char **argv, struct session_s *session, char *info);
#endif /*__COMMANDS_H__*/
