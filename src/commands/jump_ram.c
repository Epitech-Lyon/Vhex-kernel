#include <command.h>
#include <string.h>

// Internal prototype.
static void ram_jump(int argc, char **argv, struct session_s *session, char *info);

/* Define the command block into the cache section. */
CMDBLOCK("ram", &ram_jump,
"RAM command help\n"
"This function allows user to\njump into different parts of\n"
"the RAM mapped in the Virtual\nMemory.\n"
"\n"
":ram p0\n"
"Jump into the P0 area, basically"
"the user space who is managed\n"
"by the Memory Management\nUnit.\n"
"\n"
":ram p1\n"
"Jump into the RAM mapped in the P1 area; MMU is disabled and\n"
"the area is cached.\n"
"\n"
":ram p2\n"
"Jump into the RAM mapped in the P2 area; MMU is disable "
"and the area is not cached.\n"
"\n"
":ram p4\n"
"Jump into the internal chip\nmemory area; MMU is enabled\n"
"and the area is cached.\n"
);

/* ram_jump() - jump into the RAM mapped in different parts of the Virtual Memory. */
static void ram_jump(int argc, char **argv, struct session_s *session, char *info)
{
	if (argc != 2){
		strcpy(info, "argument error");
		return;
	}
	if (strcmp(argv[1], "p0") && strcmp(argv[1], "p1")
	&& strcmp(argv[1], "p2") && strcmp(argv[1], "p4")){
		sprintf(info, "bad argument");
		return;
	}
	if (!strcmp(argv[1], "p0"))
		session->anchor = 0x08000000;
	if (!strcmp(argv[1], "p1"))
		session->anchor = 0x88000000;
	if (!strcmp(argv[1], "p2"))
		session->anchor = 0xa8000000;
	if (!strcmp(argv[1], "p4"))
		session->anchor = 0xe5000000;
}
