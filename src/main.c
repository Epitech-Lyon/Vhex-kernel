#include <utils.h>
#include <string.h>
#include <syscalls.h>

//
// vhex_constructor()
// Initialize all information needed by the add-in: session,
// GetKey configuration, ...
//
static void vhex_constructor(struct vhex_s *vhex)
{
	size_t i;

	i = -1;
	memset(vhex->info, '\0', CMD_LENGHT_MAX);
	memset(vhex->insert.cmd, '\0', CMD_LENGHT_MAX);
	//TODO: Auto detect GetKey configuration !!
	vhex->insert.mode = NUMBER;
	vhex->history.list = NULL;
	vhex->history.offset = 0;
	vhex->history.deep = 0;
	vhex->current_session = 0;
	while (++i < SESSIONS_SLOT){
		vhex->session[i].mode = UNUSED;
		vhex->session[i].anchor = 0x00000000;
	}
}

//
// main()
// User entry, but we need to initialize some information before
// starting the "main loop".
//
int main(void)
{
	struct vhex_s vhex;
	unsigned int key;

	key = 0;
	vhex_constructor(&vhex);
	while (1){
		dclear();
		display_instructions(&vhex.session[vhex.current_session]);
		display_metainfos(&vhex, &vhex.session[vhex.current_session]);
		getkey(&key);
		key_handling(&vhex, &vhex.session[vhex.current_session], key);
	}
	return (0);
}
