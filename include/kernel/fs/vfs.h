#ifndef __KERNEL_FS_VFS_H__
# define __KERNEL_FS_VFS_H__

#include <stddef.h>
#include <stdint.h>
#include <kernel/types.h>
#include <kernel/fs/filesystem.h>
#include <kernel/fs/file.h>

// Internal VFS macros
#define VFS_MOUNT_ROOT	(-1)


// Constructor / destructor
extern int vfs_register_filesystem(struct file_system_type *new);

// Superblock primitives
extern int vfs_umount(char const *target);
extern int vfs_mount(const char *source, const char *target,
		const char *filesystemtype, unsigned long mountflags,
		const void *data);

// Inode operations
extern int vfs_mkdir(const char *pathname, mode_t mode);

// File operations
extern int vfs_open(FILE *file, const char *pathname, int flags);
extern ssize_t vfs_read(FILE *file, void *buf, size_t count);
extern ssize_t vfs_write(FILE *file, const void *buf, size_t count);
extern off_t vfs_lseek(FILE *file, off_t offset, int whence);

// Dentry opeations
extern struct dentry *vfs_dentry_find_first_child(struct dentry *dentry);
extern struct dentry *vfs_dentry_find_next_sibling(struct dentry *dentry);
extern struct dentry *vfs_dentry_resolve(const char *path, int mode);

#endif /*__KERNEL_FS_VFS_H__*/
