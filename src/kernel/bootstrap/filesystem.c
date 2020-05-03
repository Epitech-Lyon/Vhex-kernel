#include <kernel/devices/earlyterm.h>
#include <kernel/devices/tty.h>
#include <kernel/fs/smemfs.h>
#include <kernel/fs/gladfs.h>
#include <kernel/fs/stat.h>
#include <kernel/fs/vfs.h>

//---
//	Initialize File System and Virtual File System !
//---
void bootstrap_filesystem_init(void)
{
	extern struct file_system_type gladfs_filesystem;
	extern struct file_system_type smemfs_filesystem;

	// Internal FS init !
	earlyterm_write("Initialize File System...\n");
	gladfs_initialize();
	smemfs_initialize();

	// Initialize Virtual File System
	earlyterm_write("Init. Virtual File System...\n");
	vfs_register_filesystem(&gladfs_filesystem);
	vfs_register_filesystem(&smemfs_filesystem);

	// Creat initial FileSystem Hierarchy
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
}
