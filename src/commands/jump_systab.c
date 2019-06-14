#include <command.h>
#include <string.h>

// Internal prototype.
static void systab_jump(int argc, char **argv, struct session_s *session, char *info);

// Define the command block into the cache
CMDBLOCK("systab", &systab_jump,
"SYSTAB command help\n"
"This function allows user to\njump into the Casio \"syscall\n"
"handler\".\n"
"\n"
"Curiously, Casio doesn't use the"
"TRAPA instruction for handle\n"
"syscall, but jump at\n0x80010070.\n"
"This don't explain why we are\nalways in privileged mode, but\n"
"now we can affirm that Casio\ndo it intentionally.\n"
);

/* systab_jump() - Jump into the Casio "syscall handler". */
static void systab_jump(int argc, char **argv, struct session_s *session, char *info)
{
	if (argc != 1){
		strcpy(info, "argument error");
		return;
	}
	(void)argv;
	session->anchor = 0x80010070;
}
