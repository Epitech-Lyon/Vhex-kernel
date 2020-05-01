#ifndef __KERNEL_FS_VFS_H__
# define __KERNEL_FS_VFS_H__

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <kernel/fs/file.h>
#include <kernel/fs/filesystem.h>
#include <kernel/devices/device.h>
#include <fcntl.h>

// Internal VFS macros
#define VFS_MOUNT_ROOT				(-1)
#define VFS_DENTRY_RESOLVE_FLAG_FULL_PATH	(0)
#define VFS_DENTRY_RESOLVE_FLAG_PATHNAME	(1)



// Constructor / destructor
extern int vfs_register_filesystem(struct file_system_type *new);

// Superblock primitives
extern int vfs_umount(char const *target);
extern int vfs_mount(const char *source, const char *target,
		const char *filesystemtype, unsigned long mountflags,
		const void *data);

// Inode operations
extern int vfs_mkdir(const char *pathname, mode_t mode);
extern int vfs_mknod(const char *pathname, mode_t mode, dev_t dev);

// File operations
extern int vfs_open(FILE *file, const char *pathname, int flags);
extern ssize_t vfs_read(FILE *file, void *buf, size_t count);
extern ssize_t vfs_write(FILE *file, const void *buf, size_t count);
extern off_t vfs_lseek(FILE *file, off_t offset, int whence);
extern ssize_t vfs_pread(FILE *file, void *buf, size_t count, off_t offset);
extern ssize_t vfs_pwrite(FILE *file, const void *buf, size_t count, off_t offset);
extern int vfs_close(FILE *file);

// Dentry opeations
extern struct dentry *vfs_dentry_find_first_child(struct dentry *dentry);
extern struct dentry *vfs_dentry_find_next_sibling(struct dentry *dentry);
extern struct dentry *vfs_dentry_resolve(const char *path, int mode);
extern struct dentry *vfs_dentry_alloc(const char *name, mode_t mdode);
extern void vfs_dentry_free(struct dentry *dentry);

#endif /*__KERNEL_FS_VFS_H__*/
