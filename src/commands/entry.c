#include <command.h>
#include <string.h>
#include <errno.h>

//---
// Define all "command block". One block is composed by:
//	* id: used uniquely for the "command_find()" internal cache.
//	* name: string which contains the user's command.
//	* anchor: used if the constructor are not set, it is the new "location" of the anchor.
//	* constructor: the command constructor (and the new anchor location will be ignored).
//---
//TODO: generate automatically anonymous name, and remove "command_find()" cache.
/*CMDBLOCK(rom,		"rom",		0x00300200,	NULL);
CMDBLOCK(systab,	"systab",	0x80010070,	NULL);
CMDBLOCK(ram, 		"ram",		NULL,		&ram_jump);
CMDBLOCK(vbrjmp,	"vbrjmp",	NULL,		&vbr_jump);
CMDBLOCK(addr_jump,	"jmp",		NULL,		&address_jump);
CMDBLOCK(sysc_jump,	"syscall",	NULL,		&syscall_jump);

CMDBLOCK(exit,		"quit",		NULL,		&quit_command);
CMDBLOCK(locate,	"where",	NULL,		&where_command);
*/


//
// command_find
// Try to find the user command in the internal cache and execute constructor.
//
static int command_find(int argc, char **argv, struct session_s *session,
struct vhex_s *vhex)
{
	const struct cmd_info *command;
	int tmp;

	command = cmd_cache_find(*argv);
	if (command == NULL)
		return (EINVAL);
	if (command->constructor == NULL){
		strcpy(vhex->info, "constructor error");
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
// The goal of this part is to parse and execute user's command.
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
