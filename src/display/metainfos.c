#include <utils.h>
#include <syscalls.h>
#include <string.h>

void display_metainfos(const struct dump_s *dump, const struct session_s *session)
{
	static const char *mode_name[] = {"unused", "command", "normal", "free"};
	static const char insert_name[] = {'l', 'L', 'n', 's'};
	const void *tmp;
	char buf[32];

	// clear area
	dclear_area(0, SCREEN_HEIGHT - FONT_HEIGHT - 2, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
	dline_horizontal(SCREEN_HEIGHT - FONT_HEIGHT - 2, 0, SCREEN_WIDTH - 1);

	// Display informations
	sprintf(buf, "[%s][%d]", mode_name[session->mode], dump->current_session);
	print(SCREEN_WIDTH - (strlen(buf) * FONT_WIDTH) - 1, SCREEN_HEIGHT - FONT_HEIGHT, buf);
	if (session->mode == COMMAND)
		sprintf(buf, "[%c]:%s#", insert_name[dump->insert.mode], dump->insert.cmd);
	else {
		tmp = (*dump->info == '\0') ? dump->insert.cmd : dump->info;
		sprintf(buf, "[%c] %s", insert_name[dump->insert.mode], tmp);
	}
	print(0, SCREEN_HEIGHT - FONT_HEIGHT, buf);
}
