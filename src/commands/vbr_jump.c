#include <commands.h>
#include <string.h>

void vbr_jump(int argc, char **argv, struct session_s *session, char *info)
{
	if (argc != 2){
		strcpy(info, "arguments error");
		return;
	}
	__asm__ volatile ("stc vbr, %0" : "=r"(session->anchor));
	if (!strcmp(argv[1], "fault"))
		session->anchor += 0x100;
	if (!strcmp(argv[1], "except"))
		session->anchor += 0x400;
	if (!strcmp(argv[1], "int"))
		session->anchor += 0x600;
	session->size = 2048;
}
