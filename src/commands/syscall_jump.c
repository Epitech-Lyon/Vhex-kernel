#include <commands.h>
#include <string.h>

static INLINE int check_address(char *str)
{
	--str;
	while ((*(++str) >= '0' && *str <= '9') || (*str >= 'a' && *str <= 'f'));
	return ((*str != '\0') ? 1 : 0);
}

void syscall_jump(int argc, char **argv, struct session_s *session, char *info)
{
	uint32_t *systab;
	uint32_t id;

	if (argc != 2 || check_address(argv[1]) != 0){
		strcpy(info, "arguments error");
		return;
	}
	id = atoi_base(argv[1], 16);
	systab = (void*)(*(uint32_t *)0x8001007c);
	session->anchor = systab[id];
	session->size = 0xffffffff;
}
