#include <commands.h>
#include <string.h>

static INLINE int check_address(char *str)
{
	--str;
	while ((*(++str) >= '0' && *str <= '9') || (*str >= 'a' && *str <= 'f'));
	return ((*str != '\0') ? 1 : 0);
}

void address_jump(int argc, char **argv, struct session_s *session, char *info)
{
	if (argc != 2 || check_address(argv[1]) != 0){
		strcpy(info, "arguments error");
		return;
	}
	session->anchor = atoi_base(argv[1], 16) >> 1 << 1;
	session->size = 0xffffffff;
}
