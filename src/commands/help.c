#include <command.h>
#include <syscalls.h>
#include <string.h>
#include <errno.h>

// Internal prototype.
static void help(int argc, char **argv, struct session_s *session, char *info);

// Define help command into the cache section.
CMDBLOCK("help", &help,
"Help for Vhex version 1.0\n"
"This chapter introduces\noperations available with vhex.\n"
"\n"
"I]   Keys mapping\n"
"II]  Status bar\n"
"III] Unused mode\n"
"IV]  Command mode\n"
"V]   Normal mode\n"
"VI]  Credits\n"
"VII] Copyright\n"
"\n"
"================================"
"I] Keys mapping\n"
"Move around: [UP], [DOWN]\n"
"Change mode: [OPTN]\n"
"Letter: [ALPHA] + [A ~ Z]\n"
"Letter lock: [SHIFT] + [ALPHA]\n"
"Letter unlock: [ALPHA]\n"
"Number: [0 ~ 9]\n"
"sessions: [F0 ~ F9]\n"
"\n"
"================================"
"II] Status bar\n"
"The first information (at the\nbottom left) display is the\nkeyboard status:\n"
"   * [s] : shift mode.\n"
"   * [l] : letter mode.\n"
"   * [L] : caps lock.\n"
"   * [n] : number mode.\n"
"The second information is the\ncommand zone.\n"
"The third information is the\ncurrent mode (unused, normal,\ncommand).\n"
"And the last information is the session's id currently using.\n"
"\n"
"================================"
"III] Unused mode\n"
"This mode is set when the\nsession is not used.\n"
"Basically you can't do a lot of things, but you can enter into\n"
"the command mode using the\n[OPTN] key.\n"
"\n"
"================================"
"IV] Normal mode.\n"
"This mode allows the user to\nmove into the Virtual Memory\n"
"using [UP] and [DOWN].\n"
"Be careful, there is no security"
"and some parts of the Virtual\n"
"Memory can make your\ncalculator crash.\n"
"If a crash occurs, don't worry, you have \"just\" read a\n"
"non-readable space.\n"
"Your machine will continue to\nwork perfectly after\nthe reset.\n"
"\n"
"================================"
"V] Command mode\n"
"The command mode is probably\nthe most important part\nof Vhex.\n"
"It's the only way to move\nquickly into the different part\n"
"of the Virtual Memory.\n"
"To enter in command mode the\nuser can press the [OTPN] key,\n"
"normally a \":#\" appear and\nthe user can write\ncommand.\n"
"All commands currently\nintegrated in Vhex:\n"
"    :systab\n"
"    :vbrjmp <zone>\n"
"    :syscall <id>\n"
"    :ram <zone>\n"
"    :rom\n"
"    :help <command>\n"
"We invited you to try\n\":help <cmd>\" if you don't know how a"
" command work.\n"
"Each command typed by the user\nis stored in a history that it\n"
"can access using [UP] and\n[DOWN] keys.\n"
"\n"
"================================"
"VI] Credits\n"
"Special thank to:\n"
"    - Lephenixnoir\n"
"    - PierrotLL\n"
"    - Simon Lothar\n"
"    - Kristaba\n"
"And all communities of Planet\nCasio for his help.\n"
"specially to LephenixNoir for\nhis advices and his patience.\n"
"\n"
"================================"
"VII] Copyright\n"
"The Vhex programme use CC0\nlicence.\n"
"It allows anyone to freely\nreuse, improve or modify my\n"
"work for any purpose and\nwithout any legal restrictions, "
"except those required by law.\n");

/* get_nb_line() - Return the number of lines contain in the command's manual. */
static int get_nb_line(char const *str)
{
	int nb_line;
	int cursor;

	if (str == NULL || *str == '\0')
		return (-EBADMSG);
	cursor = 0;
	nb_line = 0;
	do {
		cursor += 1;
		if (cursor >= COLUMN_OFFSET || *str == '\n'){
			nb_line += 1;
			cursor = 0;
		}
	} while (*(++str) != '\0');
	return (nb_line + 1);
}

/* get_line() - Return the address of a target line */
static const char *get_line(char const *str, int line)
{
	int cursor;
	int nb_line;

	if (line == 0)
		return (str);
	cursor = 0;
	nb_line = 0;
	do {
		cursor += 1;
		if (cursor >= COLUMN_OFFSET || *str == '\n'){
			nb_line += 1;
			cursor = 0;
		}
	} while (*(++str) != '\0' && nb_line < line);
	return (str);
}

//
// display_text() - Display only lines which should be visible.
//
// NOTE:
// Due to the Casio's GetKey function, the font used is a little bit weird,
// some characters have not the same size, so we can't calculate easily
// the line size. (TODO: fix that).
//
static void display_text(char *buf, char const *str, int current_line)
{
	char const *text;
	int nb_line;
	int cursor;

	text = get_line(str, current_line);
	if (*text == '\0')
		return;
	nb_line = 0;
	cursor = 0;
	do {
		if (*text != '\n'){
			buf[cursor] = *text;
			cursor += 1;
		}
		if (cursor >= COLUMN_OFFSET || *text == '\n'){
			buf[cursor] = '\0';
			print(0, nb_line * FONT_HEIGHT, buf);
			nb_line += 1;
			cursor = 0;
			continue;
		}
	} while (*(++text) != '\0' && nb_line < LINE_OFFSET);
	if (cursor != 0)
		print(0, nb_line * FONT_HEIGHT, buf);
}

/* help_engine() - Display help information and handle keyboard. */
static void help_engine(char const *text, int nb_line)
{
	char buf[COLUMN_OFFSET + 1];
	unsigned int key;
	int cursor;
	int exit;

	exit = 1;
	cursor = 0;
	while (exit != 0){
		dclear();
		// Display text.
		display_text(buf, text, cursor);

		// Display exit info.
		dclear_area(0, SCREEN_HEIGHT - FONT_HEIGHT - 2, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
		dline_horizontal(SCREEN_HEIGHT - FONT_HEIGHT - 2, 0, SCREEN_WIDTH - 1);
		print(0, SCREEN_HEIGHT - FONT_HEIGHT, "press [EXIT]");
		sprintf(buf, "%d/%d", cursor + 1, nb_line + 1);
		print(SCREEN_WIDTH - (strlen(buf) * FONT_WIDTH) - 1, SCREEN_HEIGHT - FONT_HEIGHT, buf);
		getkey(&key);

		// Keys hanlding.
		if (key == KEY_UP && cursor > 0)
			cursor -= 1;
		if (key == KEY_DOWN && cursor < nb_line)
			cursor += 1;
		if (key == KEY_EXIT)
			exit = 0;
	}
}

/* help() - display command manual pages. */
static void help(int argc, char **argv, struct session_s *session, char *info)
{
	const struct cmd_info *command;
	int nb_line;

	(void)session;
	command = cmd_cache_find((argc == 1) ? "help" : argv[1]);
	if (command == NULL){
		strcpy(info, "command error");
		return;
	}
	nb_line = 0;
	if (command->man != NULL)
		nb_line = get_nb_line(command->man);
	if (command->man == NULL || nb_line < 0){
		strcpy(info, "no help entry");
		return;
	}
	nb_line -= LINE_OFFSET + 1;
	if (nb_line < 0)
		nb_line = 0;
	help_engine(command->man, nb_line);
}
