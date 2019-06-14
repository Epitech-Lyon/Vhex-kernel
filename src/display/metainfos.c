#include <utils.h>
#include <syscalls.h>
#include <string.h>

/* display_metainfo() - display the "status" bar and display user's information */
void display_metainfos(const struct vhex_s *vhex, const struct session_s *session)
{
	static const char *mode_name[] = {"unused", "command", "normal", "free"};
	static const char insert_name[] = {'l', 'L', 'n', 's'};
	const void *tmp;
	char buf[32];

	// clear area
	dclear_area(0, SCREEN_HEIGHT - FONT_HEIGHT - 2, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
	dline_horizontal(SCREEN_HEIGHT - FONT_HEIGHT - 2, 0, SCREEN_WIDTH - 1);

	// Display informations
	sprintf(buf, "[%s][%d]", mode_name[session->mode], vhex->current_session);
	print(SCREEN_WIDTH - (strlen(buf) * FONT_WIDTH) - 1, SCREEN_HEIGHT - FONT_HEIGHT, buf);
	if (session->mode == COMMAND)
		sprintf(buf, "[%c]:%s#", insert_name[vhex->insert.mode], vhex->insert.cmd);
	else {
		tmp = (*vhex->info == '\0') ? vhex->insert.cmd : vhex->info;
		sprintf(buf, "[%c] %s", insert_name[vhex->insert.mode], tmp);
	}
	print(0, SCREEN_HEIGHT - FONT_HEIGHT, buf);
}
