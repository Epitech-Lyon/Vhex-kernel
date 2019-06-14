#include <command.h>
#include <string.h>
#include <errno.h>

//
// command_find
// Try to find the user command in the internal cache and execute
// the command's function.
//
static int command_find(int argc, char **argv, struct session_s *session,
struct vhex_s *vhex)
{
	const struct cmd_block *command;
	int tmp;

	command = cmd_cache_find(*argv);
	if (command == NULL)
		return (EINVAL);
	if (command->constructor == NULL){
		strcpy(vhex->info, "function error");
		return (ENOSYS);
	}
	tmp = session->anchor;
	memset(vhex->info, '\0', CMD_LENGHT_MAX);
	command->constructor(argc, argv, session, vhex->info);
	if (*vhex->info == '\0')
		sprintf(vhex->info, "%#x", session->anchor);
	if (tmp != session->anchor)
		session->cursor = 0;
	return (0);
}

//
// command_entry()
// The goal of this part is to parse and execute the user's command.
//
void command_entry(struct session_s *session, struct vhex_s *vhex)
{
	char **argv;
	int argc;
	int ret;

	if (history_update(&vhex->history.list, vhex->insert.cmd) == 0){
		vhex->history.offset = 0;
		vhex->history.deep += 1;
	}
	ret = strtotab(&argc, &argv, vhex->insert.cmd);
	memset(vhex->insert.cmd, '\0', CMD_LENGHT_MAX);
	if (ret != 0){
		sprintf(vhex->info, "error (%d)", ret);
		return;
	}
	if (command_find(argc, argv, session, vhex) != 0)
		strcat(vhex->info, "command error");
	session->mode = (session->anchor == 0x00000000) ? UNUSED : NORMAL;
	strtotab_quit(&argc, &argv);
}
