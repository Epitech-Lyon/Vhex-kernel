#include <commands.h>
#include <string.h>
#include <errno.h>

CMDBLOCK(rom,		"rom",		0x00300200,	0xffffffff,	NULL);
CMDBLOCK(systab,	"systab",	0x80010070,	0xffffffff,	NULL);
CMDBLOCK(ram, 		"ram",		NULL,		0xffffffff,	&ram_jump);
CMDBLOCK(vbrjmp,	"vbrjmp",	NULL,		0xffffffff,	&vbr_jump);
CMDBLOCK(addr_jump,	"jmp",		NULL,		0xffffffff,	&address_jump);
CMDBLOCK(sysc_jump,	"syscall",	NULL,		0xffffffff,	&syscall_jump);


//TODO:
//	- dynamique allocation cache.
//	- rename function.
static int find_command(int argc, char **argv, struct session_s *session,
struct dump_s *dump)
{
	static const struct cmd_info *cache[] = {
		&addr_jump, &vbrjmp, &systab, &sysc_jump, &ram, &rom, NULL
	};
	size_t i;

	i = -1;
	while (cache[++i] != NULL && strcmp(cache[i]->name, *argv));
	if (cache[i] == NULL)
		return (EBADMSG);
	if (cache[i]->init == NULL){
		session->cursor = 0;
		session->anchor = cache[i]->update.anchor;
		session->size = cache[i]->update.size;
		sprintf(dump->info, "%#x", session->anchor);
		return (0);
	}
	cache[i]->init(argc, argv, session, dump->info);
	sprintf(dump->info, "%#x", session->anchor);
	return (0);
}

void command_entry(struct session_s *session, struct dump_s *dump)
{
	char **argv;
	int argc;
	int ret;

	if (history_update(&dump->history.list, dump->insert.cmd) == 0){
		dump->history.offset = 0;
		dump->history.deep += 1;
	}
	ret = strtotab(&argc, &argv, dump->insert.cmd);
	memset(dump->insert.cmd, '\0', CMD_LENGHT_MAX);
	if (ret != 0){
		sprintf(dump->info, "error (%d)", ret);
		return;
	}
	if (find_command(argc, argv, session, dump) != 0)
		strcat(dump->info, "command error");
	session->mode = (session->anchor == 0x00000000) ? UNUSED : NORMAL;
	session->cursor = 0;
	strtotab_quit(&argc, &argv);
}
