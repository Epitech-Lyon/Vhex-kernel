#ifndef __KERNEL_FS_FS_H__
# define __KERNEL_FS_FS_H__

#include <stddef.h>
#include <stdint.h>
#include <asm/types.h>

// FS flags
#define FS_RDWR		(0x01)
#define FS_RDONLY	(0x02)
#define FS_NO_DEVICE	(0x02)


// Internal file operations
struct file_system_operations
{
	void *(*mount)(void);
	int (*umount)(void);
};

// Internal super block operations
// TODO
/*struct super_operations
{
	// Inode management part.
//	void *(*alloc_inode)(struct super_block *sb);
//	void (*destroy_inode)(void *inode);

	// Internal FS primitives
//	int (*sync_fs)(struct super_block *sb, int wait);
//	int (*statfs) (struct dentry *, struct kstatfs *);
};*/

// Internal Inode operations
struct inode_operations
{
	// Inode walk part.
	void *(*find_next_sibling)(void *inode);
	void *(*find_first_child)(void *inode);
	void *(*find_parent)(void *inode);
	
	// Inode memory part.
	void *(*create)(void *parent_inode, char const *name, mode_t mode);
	void *(*mkdir)(void *parent_inode, char const *name, mode_t mode);
	int (*rmdir)(void *inode);
	int (*rename)(void *indde, char const *name);

	// Inode informartions part.
	int (*get_name)(void *inode, char *buf, size_t count);
	mode_t (*get_mode)(void *inode);
};

struct file_operations
{
	ssize_t (*read)(void *inode, void *buf, size_t count, off_t pos);
	ssize_t (*write)(void *inode, const void *buf, size_t count, off_t pos);
	void (*ioctl)(void *inode, uint32_t cmd, ...);
};

// Based on UNIX Kernel 2.6.39
struct file_system_type
{
	const char *fs_name;
	uint8_t fs_flags;
	struct file_system_operations filesystem_operations;
	struct file_operations file_operations;
	struct inode_operations inode_operations;
	struct file_system_type *next;
};

#define VFS_DENTRY_NAME_LENGHT 24

// Used to walk into VFS
struct dentry
{
	// File name.
	char name[VFS_DENTRY_NAME_LENGHT];

	// Internal inode 
	void *inode;

	// Internal counter used by opne() and close()
	uint32_t counter;

	// Internal file informations
	mode_t mode;

	// Linked list
	struct dentry *parent;
	struct dentry *child;
	struct dentry *next;

	// Mount point
	struct {
		void *inode;
		struct file_operations *file_op;
		struct inode_operations *inode_op;
	} mnt;

	// Internal dentry openrations
	struct {
		struct file_operations *file_op;
		struct inode_operations *inode_op;
	} dentry_op;
};


#endif /*__KERNEL_FS_FS_H__*/
