#include <utils.h>
#include <syscalls.h>
#include <commands.h>
#include <string.h>

static int check_session(unsigned int key, struct dump_s *dump)
{
	static const unsigned int keys_switch[SESSIONS_SLOT] = {
		KEY_F1, KEY_F2, KEY_F3,
		KEY_F4, KEY_F5, KEY_F6
	};
	size_t i;

	i = -1;
	while (++i < SESSIONS_SLOT && key != keys_switch[i]);
	if (i < SESSIONS_SLOT){
		dump->current_session = i;
		return (0);
	}
	return (-1);
}

static int check_insert(unsigned int key, struct dump_s *dump, struct session_s *session)
{
	if (!(key == KEY_OPTN || key == KEY_SHIFT || key == KEY_ALPHA))
		return (-1);
	if (dump->insert.mode == LETTER)
		dump->insert.mode = NUMBER;
	if (key == KEY_OPTN){
		if (session->mode != COMMAND){
			memset(dump->info, '\0', CMD_LENGHT_MAX);
			session->mode = COMMAND;
			save_window(1);
		}
		else
			//TODO: restore FREE mode ?
			session->mode = (session->anchor == 0x00000000) ? UNUSED : NORMAL;
	}
	if (key == KEY_SHIFT)
		dump->insert.mode = (dump->insert.mode == SHIFT) ? NUMBER : SHIFT;
	if (key == KEY_ALPHA){
		if (dump->insert.mode == SHIFT){
			dump->insert.mode = CAPS_LOCK;
			return (0);
		}
		if (dump->insert.mode == CAPS_LOCK){
			dump->insert.mode = NUMBER;
			return (0);
		}
		dump->insert.mode = LETTER;
	}
	return (0);
}

//TODO: find better way to do the job (?)
static int check_cmd(unsigned int key, struct dump_s *dump, struct session_s *session)
{
	static const unsigned int keys_alpha[] = {
		KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H,
		KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P,
		KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X,
		KEY_Y, KEY_Z
	};
	static const unsigned int keys_number[] = {
		KEY_0, KEY_1, KEY_2, KEY_3, KEY_4,
		KEY_5, KEY_6, KEY_7, KEY_8, KEY_9
	};
	unsigned int *cache;
	size_t size;
	size_t end;
	size_t i;
	char buf;

	if (session->mode != COMMAND)
		return (-1);
	i = -1;
	size = strlen(dump->insert.cmd);
	end = (dump->insert.mode == NUMBER) ? 10 : 26; 
	buf = (dump->insert.mode == NUMBER) ? '0' : 'a';
	cache = (dump->insert.mode == NUMBER) ? (void *)keys_number : (void *)keys_alpha;
	while (++i < end && key != cache[i]);
	if (i >= end)
		return (-1);
	if (size < CMD_LENGHT_MAX){
		buf += i;
		strcat(dump->insert.cmd, &buf);
	}
	dump->history.offset = 0;
	return (0);
}

static int check_arrow(unsigned int key, struct dump_s *dump, struct session_s *session)
{
	if (key != KEY_UP && key != KEY_DOWN)
		return (-1);
	if (session->mode == COMMAND
	&& ((key == KEY_UP && dump->history.offset < dump->history.deep)
	|| (key == KEY_DOWN && dump->history.offset > 0))){
		if (dump->history.offset == 0)
			strcpy(dump->insert.backup, dump->insert.cmd);
		dump->history.offset += (key == KEY_UP) ? 1 : -1;
		if (dump->history.offset > 0)
			strcpy(dump->insert.cmd,
			history_get(dump->history.list, dump->history.offset - 1));
		else
			strcpy(dump->insert.cmd, dump->insert.backup);
	}
	if (session->mode != UNUSED && session->mode != COMMAND){
		if (key == KEY_UP && session->cursor > 0)
			session->cursor -= INSTRUCTION_SIZE;
		if (key == KEY_DOWN && session->cursor < session->size - LINE_OFFSET)
			session->cursor += INSTRUCTION_SIZE;
	}
	return (0);
}

void key_handling(struct dump_s *dump, struct session_s *session, unsigned int key)
{
	size_t size;

	if (check_session(key, dump) == 0
	|| check_insert(key, dump, session) == 0
	|| check_arrow(key, dump, session) == 0
	|| check_cmd(key, dump, session) == 0
	|| session->mode != COMMAND)
		return;
	if (key == KEY_DEL || key == KEY_INS){
		size = strlen(dump->insert.cmd);
		if (size > 0)
			strncpy(dump->insert.cmd, dump->insert.cmd, size - 1);
	}
	if (key == KEY_SPACE)
		strcat(dump->insert.cmd, " ");
	if (key == KEY_QUOTE)
		strcat(dump->insert.cmd, "\"");
	if (key == KEY_ENTER){
		dump->insert.mode = NUMBER;
		command_entry(session, dump);
	}
}
