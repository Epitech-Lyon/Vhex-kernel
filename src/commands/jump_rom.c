#include <command.h>
#include <string.h>

// internal prototype.
static void rom_jump(int argc, char **argv, struct session_s *session, char *info);

// Define the command into the cache.
CMDBLOCK("rom", &rom_jump,
"ROM command help\n"
"This command allows user to\njump into different parts of\n"
"the ROM mapped in the Virtual\nMemory.\n"
"\n"
":rom addin\n"
"Jump into the dynamic add-in\nmapping area.\n"
"Normaly you can see the\nVhex binary :)\n"
"(see \":help jmp\" to get the\nVirtual Memory map)\n"
"\n"
":rom p1\n"
"Jump into the ROM mapped in\nthe P1 area.\n"
"MMU is disable and the area\nis cached.\n"
"\n"
"rom p2\n"
"Jump into the ROM mapped in\nthe P2 area.\n"
"MMU is disable and the area\nis not cached.\n"
);

/* rom_jump() - jump into the start of the ROM */
static void rom_jump(int argc, char **argv, struct session_s *session, char *info)
{
	if (argc != 2){
		strcpy(info, "argument error");
		return;
	}
	if (strcmp(argv[1], "addin") && strcmp(argv[1], "p1") && strcmp(argv[1], "p2")){
		sprintf(info, "bad argument");
		return;
	}
	if (!strcmp(argv[1], "addin"))
		session->anchor = 0x00300200;
	if (!strcmp(argv[1], "p1"))
		session->anchor = 0x80000000;
	if (!strcmp(argv[1], "p2"))
		session->anchor = 0xa0000000;
}
