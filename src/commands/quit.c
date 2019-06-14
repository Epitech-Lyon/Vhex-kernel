#include <command.h>
#include <string.h>

// internal prototype.
static void quit(int argc, char **argv, struct session_s *session, char *info);

// Define the command into the cache.
CMDBLOCK("quit", &quit, NULL);

/* quit() - switch to UNUSED mode */
static void quit(int argc, char **argv, struct session_s *session, char *info)
{
	(void)argc;
	(void)argv;
	strcpy(info, "exit");
	session->mode = UNUSED;
	session->anchor = 0x00000000;
}
