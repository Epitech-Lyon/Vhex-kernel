#include <command.h>
#include <string.h>

// internal prototype.
static void where(int argc, char **argv, struct session_s *session, char *info);

// Define the command into the cache.
CMDBLOCK("where", &where, NULL);

/* where() - display the user positision in the Virtual Memory. */
static void where(int argc, char **argv, struct session_s *session, char *info)
{
	(void)argc;
	(void)argv;
	sprintf(info, "%#x", session->anchor + (session->cursor << 1));
}
