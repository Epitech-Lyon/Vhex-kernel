#include <commands.h>
#include <string.h>

void ram_jump(int argc, char **argv, struct session_s *session, char *info)
{
	if (argc != 2){
		strcpy(info, "arguments error");
		return;
	}
	if (!strcmp(argv[1], "p0"))
		session->anchor = 0x08100000;
	if (!strcmp(argv[1], "p1"))
		session->anchor = 0x88000000;
	if (!strcmp(argv[1], "p2"))
		session->anchor = 0xa8000000;
	if (!strcmp(argv[1], "p2"))
		session->anchor = 0xe6000000;
	//FIXME: sh3 size is 0x40000;
	session->size = 0x80000;
}
