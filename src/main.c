#include <utils.h>
#include <string.h>
#include <syscalls.h>

//
// dump_constructor()
// Initialize all information needed by the add-in: session,
// GetKey configuration, ...
//
static void dump_constructor(struct dump_s *dump)
{
	size_t i;

	i = -1;
	memset(dump->info, '\0', CMD_LENGHT_MAX);
	memset(dump->insert.cmd, '\0', CMD_LENGHT_MAX);
	//TODO: Auto detect GetKey configuration !!
	dump->insert.mode = NUMBER;
	dump->history.list = NULL;
	dump->history.offset = 0;
	dump->history.deep = 0;
	dump->current_session = 0;
	while (++i < SESSIONS_SLOT){
		dump->session[i].mode = UNUSED;
		dump->session[i].anchor = 0x00000000;
	}
}

//
// main()
// "user" entry, but we need initialize some information before
// starting the "main loop".
//
int main(void)
{
	struct dump_s dump;
	unsigned int key;

	key = 0;
	dump_constructor(&dump);
	while (1){
		dclear();
		display_instructions(&dump.session[dump.current_session]);
		display_metainfos(&dump, &dump.session[dump.current_session]);
		getkey(&key);
		key_handling(&dump, &dump.session[dump.current_session], key);
	}
	return (0);
}
