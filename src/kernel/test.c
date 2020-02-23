#include <kernel/fs/vfs.h>
#include <kernel/devices/tty.h>
#include <kernel/memory.h>
#include <kernel/util/debug.h>
#include <kernel/util/draw.h>
#include <kernel/util/string.h>

// Tree-wrapper 
static void vfs_test(struct dentry *node, int level)
{
	// Check error.
	if (node == NULL)
		return;

	// Space management.
	for (int i = 0 ; i < level ; i++)
		tty_write(NULL, " ", 1);

	// Display name
	tty_write(NULL, node->name, strlen(node->name));
	tty_write(NULL, "\n", 1);

	// Walk into child and sibling
	vfs_test(vfs_dentry_find_first_child(node), level + 1);
	vfs_test(vfs_dentry_find_next_sibling(node), level);
}

void kernel_test(void)
{
	extern struct dentry *vfs_root_node;
	char path[] = "/../dev/./../dev/////tty";
	FILE file;

	// Debug !
	kvram_clear();
	printk(0, 0, "Kernel test mode entry !");
	printk(0, 1, "Check VFS...");
	kvram_display();
	DBG_WAIT;

	// Check root node
	printk(0, 2, "vfs_root_node = %p", vfs_root_node);
	printk(0, 3, "vfs_root_node = %s$", vfs_root_node->name);
	printk(0, 4, "vfs_root_node = %p", vfs_root_node->child);
	printk(0, 5, "vfs_root_node = %p", vfs_root_node->next);
	kvram_display();
	DBG_WAIT;

	// VFS test
	tty_open(0, 0);
	vfs_test(vfs_root_node, 0);
	tty_write(NULL, "FINI !\n", 7);
	DBG_WAIT;
	DBG_WAIT;

	// Try to open file using VFS
	if (vfs_open(&file, path, 0))
	{
		kvram_clear();
		printk(0, 0, "vfs_open(): error !");
		printk(0, 1, "path %s$", path);
		kvram_display();
		while (1) { __asm__ volatile ("sleep"); }
	}

	// Display file informations
	kvram_clear();
	printk(0, 0, "vfs_open: success !");
	printk(0, 1, "path: %s$", path);
	printk(0, 2, "file->private: %p", file.private);
	printk(0, 3, "file->file_op: %p", file.file_op);
	kvram_display();
	while (1);
}
