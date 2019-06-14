#include <command.h>
#include <string.h>

// Internal prototype.
static void syscall_jump(int argc, char **argv, struct session_s *session, char *info);

/* Define command block into the cache section. */
CMDBLOCK("syscall", &syscall_jump,
"SYSCALL command help\n"
"Take one parameter: syscall id.\n"
"This command will jump at the\nsyscall's code entry.\n"
"\n"
"Syscall list (not exhaustive):\n"
"0x013 : GLibAddinAppExecutionCheck\n"
"0x014 : GLibGetAddinLibInfo\n"
"0x014 : GLibGetOSVersionInfo\n"
"0x01c : Bdisp_WriteGraph_VRAM\n"
"0x01d : Bdisp_WriteGraph_DD\n"
"0x01e : Bdisp_WriteGraph_DDVRAM\n"
"0x022 : Bdisp_ReadArea_VRAM\n"
"0x023 : Bdisp_ReadArea_DD_OS\n"
"0x024 : Bdisp_GetDisp_DD\n"
"0x026 : DD_GET\n"
"0x028 : Bdisp_PutDisp_DD\n"
"0x030 : Bdisp_DrawLineVRAM\n"
"0x031 : Bdisp_ClearLine_VRAM\n"
"0x118 : Bcre_cychdr\n"
"0x119 : Bdel_cychdr\n"
"0x142 : Bdisp_AllClr_DD\n"
"0x143 : Bdisp_AllClr_VRAM\n"
"0x144 : Bdisp_AllClr_DDVRAM\n"
"0x145 : Bdisp_GetDisp_VRAM\n"
"0x147 : Bdisp_SetPoint_DD\n"
"0x148 : Bdisp_SetPoint_DDVRAM\n"
"0x149 : Bdisp_GetPoint_VRAM\n"
"0x14a : Bdisp_AreaClr_DD\n"
"0x14b : Bdisp_AreaClr_VRAM\n"
"0x14c : Bdisp_AreaClr_DDVRAM\n"
"0x14d : Bdisp_AreaReverseVRAM\n"
"0x11a : Bsta_cychdr\n"
"0x11b : Bstp_cychdr\n"
"0x11f : Bdisp_PutDispArea_DD\n"
"0x1e7 : BfileFLS_CloseFile\n"
"0x218 : flsFindCLose\n"
"0x242 : Bkey_Set_RepeatTime\n"
"0x243 : Bkey_Get_RepeatTime\n"
"0x244 : Bkey_Set_RepeatTime_Default\n"
"0x247 : Bkey_GetKeyWait\n"
"0x24c : Chattering\n"
"0x374 : BMCSRenameVariable\n"
"0x3ed : SetFlagPaturn\n"
"0x3fa : Hmem_SetMMU\n"
"0x420 : BSrl_DummyWAIT\n"
"0x42c : Bfile_OpenFile_OS\n"
"0x42d : Bfile_CloseFile_OS\n"
"0x42e : Bfile_GetMediaFree_OS\n"
"0x42f : Bfile_GetFileSize_OS\n"
"0x431 : Bfile_SeekFile_OS\n"
"0x432 : Bfile_ReadFile_OS\n"
"0x434 : Bfile_CreateEntry\n"
"0x435 : Bfile_WriteFile_OS\n"
"0x439 : Bfile_DeleteEntry\n"
"0x43b : Bfile_FindFirst\n"
"0x43d : Bfile_FindClose\n"
"0x43c : Bfile_FindNext\n"
"0x462 : GetAppName\n"
"0x494 : CallBackAtQuitMainFunction\n"
"0x807 : Locate_OS\n"
"0x82b : MCSPutVar2\n"
"0x830 : MCSOvwDat2\n"
"0x836 : MCSDelVar2\n"
"0x840 : MCSGetDlen2\n"
"0x841 : MCSGetData1\n"
"0x844 : MCSGetCapa\n"
"0x8fe : PopUpWin\n"
"0x808 : Print\n"
"0x809 : PrintRev\n"
"0x80b : PrintRev\n"
"0x80d : PrintRLine\n"
"0x80a : PrintC\n"
"0x80c : PrintLine\n"
"0x813 : SaveDisp\n"
"0x814 : RestoreDisp\n"
"0x90f : GetKey\n"
"0x9ad : PrintXY\n"
"0xacc : free\n"
"0xacd : malloc\n"
"0xc4f : PrintMiniSd\n"
"0xe6b : calloc\n"
"0xe6d : realloc\n"
"0x1032 : Bkey_GetKeyTableInfo\nJumpFunc\n"
);

static INLINE int check_address(char *str)
{
	--str;
	while ((*(++str) >= '0' && *str <= '9') || (*str >= 'a' && *str <= 'f'));
	return ((*str != '\0') ? 1 : 0);
}

//
// syscall_jump()
// Casio doesn't use the TRAPA instruction (software interruption), for switch from
// user mode to privileged mode. They just jump always at 0x80010070 and use the
// table of syscall addresses to redirect the jump. (and the table address is always
// stored at 0x8001007c).
//
static void syscall_jump(int argc, char **argv, struct session_s *session, char *info)
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
}
