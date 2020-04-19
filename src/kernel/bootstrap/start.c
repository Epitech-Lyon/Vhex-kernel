#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
// Hardware
#include <kernel/hardware/mpu.h>
// Internal helpers
#include <kernel/util/atomic.h>
#include <kernel/util/casio.h>
// Modules
#include <kernel/context.h>
#include <kernel/process.h>
#include <kernel/syscall.h>
#include <kernel/scheduler.h>
#include <kernel/loader.h>
// Devices
#include <kernel/devices/tty.h>
#include <kernel/devices/earlyterm.h>
// File System
#include <kernel/fs/vfs.h>
#include <kernel/fs/stat.h>
#include <kernel/fs/smemfs.h>
#include <kernel/fs/gladfs.h>
// Libs
#include <lib/display.h>
#include <lib/string.h>

// Internal symbols
mpu_t current_mpu = MPU_UNKNOWN;

// Internal hardware context.
fx9860_context_t casio_context;
fx9860_context_t vhex_context;

// Symbols defined by the linker
extern uint32_t bbss;
extern uint32_t sbss;
extern uint32_t bdata_ram;
extern uint32_t bdata_rom;
extern uint32_t sdata;
extern uint32_t bvhex_ram;
extern uint32_t bvhex_rom;
extern uint32_t svhex;
extern uint32_t bubc_ram;
extern uint32_t bubc_rom;
extern uint32_t subc;
extern uint32_t brom;
extern uint32_t srom;
extern uint32_t bctors;
extern uint32_t ectors;
extern uint32_t bdtors;
extern uint32_t edtors;

// Internal functions.
extern void vhex_context_set(void);
extern void kernel_switch(common_context_t *context);
extern mpu_t mpu_get(void);
extern int main(void);

// Internal object
extern struct file_system_type gladfs_filesystem;
extern struct file_system_type smemfs_filesystem;


//
// rom_explore() - explore all add-in ROM part.
// @note:
//	For now, we can not use the MMU so we can
// not handle TLB miss. Casio load only the first 4ko
// of the add-in. To avoid TLB miss we explore all
// add-in binary to force Casio to generate all pages
// into the TLB.
//
static void rom_explore(volatile void *rom, int32_t size)
{
	uint8_t unused;

	(void)unused;
	while (size >= 0)
	{
		unused = *(volatile uint8_t*)rom;
		size = size - 1024;
		rom = rom + 1024;
	}
}

/* section_execute() - Used to execute contructors and destructors */
static void section_execute(void *bsection, void *esection)
{
	while (bsection < esection)
	{
		((void (*)(void))*((void**)bsection))();
		bsection = bsection + sizeof(void (*)(void));
	}
}


/* start() - Kernel entry point */
__attribute__((section(".pretext")))
int start(void)
{
	//--
	//	Bootstrap part !
	//--

	// Wipe .bss section and dump .data / Vhex sections
	memset(&bbss, 0x00, (size_t)&sbss);
	memcpy(&bdata_ram, &bdata_rom, (size_t)&sdata);

	// Check MPU hardware.
	current_mpu = mpu_get();
	if (current_mpu != MPU_SH7305)
		return (0);

	// Load UBC / VBR space.
	memcpy(&bubc_ram, &bubc_rom, (size_t)&subc);
	memcpy(&bvhex_ram, &bvhex_rom, (size_t)&svhex);

	// Casio do not load all add-in
	// page. So we need to "force" update TLB
	// before switching the VBR.
	rom_explore(&brom, (int32_t)&srom);

	// TODO: load driver on-the-fly
	extern void screen_driver_load(void);
	screen_driver_load();

	// Start early terminal device
	// This device is used by the kernel to display
	// some logs on screen
	if (earlyterm_init() != 0)
		return (-1);
	earlyterm_clear();
	earlyterm_write("Kernel initialisation...\n");

	// Execute constructor.
	section_execute(&bctors, &ectors);



	// Save Casio's hardware context and set
	// Vhex hardware context.
	// @note:
	// 	This operation should be atomic
	// because if an interruption or exception
	// occur during the hardware context change
	// the calculator will crash.
	// And this is why between each `atomic_start`
	// and `atomic_end()` the code *SHOULD* be
	// exception safe.
	earlyterm_write("Environment switch...\n");
	atomic_start();
	fx9860_context_save(&casio_context);
	vhex_context_set();
	atomic_stop();

	//---
	//	File System part !
	//---

	// Internal FS init !
	earlyterm_write("Initialize File System...\n");
	gladfs_initialize();
	smemfs_initialize();

	// Initilize Virtual File System
	earlyterm_write("Init. Virtual File System...\n");
	vfs_register_filesystem(&gladfs_filesystem);
	vfs_register_filesystem(&smemfs_filesystem);

	// Creat initial file tree
	earlyterm_write("Create Filesystem Hierarchy...\n");
	vfs_mount(NULL, NULL, "gladfs", VFS_MOUNT_ROOT, NULL);
	vfs_mkdir("/dev", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	vfs_mkdir("/home", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	vfs_mkdir("/mnt", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	vfs_mkdir("/mnt/casio", S_IRUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	vfs_mount(NULL, "/mnt/casio", "smemfs", /*MS_RDONLY*/0, NULL);
	
	// Add devices
	earlyterm_write("Add devices...\n");
	vfs_mknod("/dev/tty", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH,
			dev_make_major(TTY_DEV_MAJOR));


	//---
	//	Start first process !
	//---
	
	// Load programe.
	earlyterm_write("Create first process...\n");
	struct process *vhex_process = process_create();
	if (vhex_process == NULL || loader(vhex_process, "/mnt/casio/VHEX/shell.elf") != 0)
	{
		// Display message.
		earlyterm_clear();
		earlyterm_write("Vhex fatal error !\n");
		earlyterm_write("Unable to create the first process !\n");
		if (vhex_process->context.spc == 0xffffffff)
			earlyterm_write("process_create() error\n");
		else
			earlyterm_write("File \"VHEX/shell.elf\" not found !\n");
		earlyterm_write("Press [MENU key]...\n");

		// Restore Casio context.
		fx9860_context_restore(&casio_context);
		atomic_stop();

		// Casio VRAM workaround
		// @note: GetKey will call Bdisp_PutDD_VRAM(),
		// so save the current internal Video RAM data into
		// Casio's VRAM.
		extern struct earlyterm earlyterm;
		memcpy(casio_Bdisp_GetVRAM(), earlyterm.display.vram, 1024);

		// Wait MENU key.
		unsigned int key;
		while (1)
		{
			casio_GetKey(&key);
		}
	}


	//---
	//	Initialize sheduler !!
	//---
	earlyterm_write("Initialize scheduler...\n");
	sched_initialize();
	sched_add_task(vhex_process);
	sched_start();

	// normally the kernel SHOULD / CAN not arrive here.
	earlyterm_write("Kernel job fini !");
	while (1) { __asm__ volatile ("sleep"); }
}
