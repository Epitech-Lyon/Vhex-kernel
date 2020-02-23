#ifndef __KERNEL_FS_GLADFS_H__
# define __KERNEL_FS_GLADFS_H__

#include <stdint.h>
#include <stddef.h>
#include <kernel/util/types.h>

# define GLADFS_INODE_NAME_LENGHT	(16)

//#define GLADFS_INODE_READ		(0x00)
//#define GLADFS_INODE_WRITE		(0x01)
//#define GLADFS_INODE_EXEC		(0x02)

#define GLADFS_INODE_TYPE_FILE		(0x01)
#define GLADFS_INODE_TYPE_PARENT	(0x02)
#define GLADFS_INODE_TYPE_ROOT		(0x04)

struct gladfs_fragment_data_s
{
	uint32_t next;
	uint32_t data_size;
	uint32_t data_used;
	uint32_t data_entry;
};

struct gladfs_inode_s
{
	// Internal file informations
	char name[GLADFS_INODE_NAME_LENGHT];
	mode_t mode;

	// File organisations
	struct gladfs_inode_s *parent;
	struct gladfs_inode_s *children;
	struct gladfs_inode_s *next;

	// Size / Data informations
	size_t size;
	uint32_t fragnumber;
	struct gladfs_fragment_data_s *fragdata;
};

struct gladfs_superblock_s
{
	struct gladfs_inode_s *root_inode;
	uint32_t block_size;
	struct {
		struct gladfs_inode_s *(*alloc_inode)(const char *name, mode_t mode);
		int (*destroy_inode)(struct gladfs_inode_s *inode);
		int (*alloc_fragdata)(struct gladfs_fragment_data_s **parent, int nb_block);
		int (*destroy_fragdata)(struct gladfs_fragment_data_s *parent);
	} super_op;
};

// Constructor
extern void gladfs_initialize(void);

// FS openrations
extern void *gladfs_mount(void);
extern int gladfs_umount(void);

// SuperBlock openrations
extern int gladfs_destroy_inode(struct gladfs_inode_s *inode);
extern struct gladfs_inode_s *gladfs_alloc_inode(const char *name, mode_t mode);
extern int gladfs_alloc_fragdata(struct gladfs_fragment_data_s **parent, int nb_block);
extern int gladfs_destroy_fragdata(struct gladfs_fragment_data_s *fragment);

// Inode operations
extern void *gladfs_mkdir(void *inode_parent, const char *name, mode_t mode);
extern void *gladfs_creat(void *inode_parent, const char *name, mode_t mode);
extern void *gladfs_find_next_sibling(void *inode);
extern void *gladfs_find_first_child(void *inode);
extern void *gladfs_find_parent(void *inode);
extern int gladfs_get_name(void *inode, char *name, size_t count);
extern mode_t gladfs_get_mode(void *inode);

// File operations
extern ssize_t gladfs_write(void *inode, const void *buf, size_t count, off_t pos);
extern ssize_t gladfs_read(void *inode, void *buf, size_t count, off_t pos);

#endif /*__KERNEL_FS_GLADFS_H__*/
