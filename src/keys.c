#include <utils.h>
#include <syscalls.h>
#include <command.h>
#include <string.h>

//
// check_session()
// Check if the pressed key is [Fx], and switch the current session.
//
static int check_session(unsigned int key, struct vhex_s *vhex)
{
	static const unsigned int keys_switch[SESSIONS_SLOT] = {
		KEY_F1, KEY_F2, KEY_F3,
		KEY_F4, KEY_F5, KEY_F6
	};
	size_t i;

	i = -1;
	while (++i < SESSIONS_SLOT && key != keys_switch[i]);
	if (i < SESSIONS_SLOT){
		vhex->current_session = i;
		return (0);
	}
	return (-1);
}

//
// check_special()
// As explained below, the Casio getkey function has an internal status.
// Here we check if a "special" keys are pressed and update internal Vhex key's status.
//
static int check_special(unsigned int key, struct vhex_s *vhex, struct session_s *session)
{
	if (!(key == KEY_OPTN || key == KEY_SHIFT || key == KEY_ALPHA))
		return (-1);
	if (vhex->insert.mode == LETTER)
		vhex->insert.mode = NUMBER;
	if (key == KEY_OPTN){
		if (session->mode != COMMAND){
			memset(vhex->info, '\0', CMD_LENGHT_MAX);
			session->mode = COMMAND;
			save_window(1);
		}
		else
			session->mode = (session->anchor == 0x00000000) ? UNUSED : NORMAL;
	}
	if (key == KEY_SHIFT)
		vhex->insert.mode = (vhex->insert.mode == SHIFT) ? NUMBER : SHIFT;
	if (key == KEY_ALPHA){
		//TODO: update this.
		if (vhex->insert.mode == SHIFT)
			vhex->insert.mode = CAPS_LOCK;
		else if (vhex->insert.mode == CAPS_LOCK)
			vhex->insert.mode = NUMBER;
		else
			vhex->insert.mode = LETTER;
	}
	return (0);
}

//
// check_alphanum()
// Check if the key pressed is an alphanumeric character and update the user command.
//
/* TODO: find better way to do the job (?) */
static int check_alphanum(unsigned int key, struct vhex_s *vhex, struct session_s *session)
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
	size = strlen(vhex->insert.cmd);
	end = (vhex->insert.mode == NUMBER) ? 10 : 26; 
	cache = (vhex->insert.mode == NUMBER) ? (void *)keys_number : (void *)keys_alpha;
	while (++i < end && key != cache[i]);
	if (i >= end)
		return (-1);
	if (size < CMD_LENGHT_MAX){
		buf = (vhex->insert.mode == NUMBER) ? '0' + i : 'a' + i;
		strcat(vhex->insert.cmd, &buf);
	}
	vhex->history.offset = 0;
	return (0);
}

//
// check_arrow()
// Check if the key pressed is an arrow key.
// Here we just check left and right key, but they do not have the same action when
// the mode is COMMAND or not; basically:
// 	* COMMAND	: Udpate the user's command using the history.
// 	* NORMAL	: Move the anchor cursor.
// 	* UNUSED	: Do nothing.
//
static int check_arrow(unsigned int key, struct vhex_s *vhex, struct session_s *session)
{
	if (key != KEY_UP && key != KEY_DOWN)
		return (-1);
	if (session->mode == COMMAND
	&& ((key == KEY_UP && vhex->history.offset < vhex->history.deep)
	|| (key == KEY_DOWN && vhex->history.offset > 0))){
		if (vhex->history.offset == 0)
			strcpy(vhex->insert.backup, vhex->insert.cmd);
		vhex->history.offset += (key == KEY_UP) ? 1 : -1;
		if (vhex->history.offset > 0)
			strcpy(vhex->insert.cmd,
			history_get(vhex->history.list, vhex->history.offset - 1));
		else
			strcpy(vhex->insert.cmd, vhex->insert.backup);
	}
	if (session->mode == NORMAL)
			session->cursor += (key == KEY_UP) ? -1 : 1;
	return (0);
}

//
// key_handling()
// We need to do several things here; the Casio getkey function handles [SHIFT], [ALPHA]
// and [MENU] key and it is able to do some special thing using the key combination like
// shutdown the calculator, change the brightness or return to the main menu using.
// But when the user press on [SHIFT] or [ALPHA], getkey() return not the same value for
// the same key (and we cannot know the "status").
// So we need to "emulate" the getkey's status and execute the appropriate action with
// the key returned by Casio.
//
void key_handling(struct vhex_s *vhex, struct session_s *session, unsigned int key)
{
	size_t size;

	if (check_session(key, vhex) == 0
	|| check_special(key, vhex, session) == 0
	|| check_arrow(key, vhex, session) == 0
	|| check_alphanum(key, vhex, session) == 0
	|| session->mode != COMMAND)
		return;
	if (key == KEY_DEL || key == KEY_INS){
		size = strlen(vhex->insert.cmd);
		if (size > 0)
			strncpy(vhex->insert.cmd, vhex->insert.cmd, size - 1);
	}
	if (key == KEY_SPACE)
		strcat(vhex->insert.cmd, " ");
	if (key == KEY_QUOTE)
		strcat(vhex->insert.cmd, "\"");
	if (key == KEY_ENTER){
		vhex->insert.mode = NUMBER;
		command_entry(session, vhex);
	}
}
