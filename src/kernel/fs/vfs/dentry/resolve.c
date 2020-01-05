#include <kernel/fs/vfs.h>
#include <kernel/fs/stat.h>
#include <kernel/util.h>

//TODO add '.' handling.
//TODO add '..' handling.
//TODO add '//' handling
struct dentry *vfs_dentry_resolve(const char *path, int mode)
{
	extern struct dentry *vfs_root_node;
	struct dentry *curr_dentry;
	struct dentry *old_dentry;
	int name_lenght;
	char name[24];

	// Check VFS validity
	if (vfs_root_node == NULL){
		kvram_clear();
		printk(0, 0, "VFS root  error !");
		kvram_display();
		DBG_WAIT;
		return (NULL);
	}

	// Get start inode
	if (path[0] != '/'){
		// TODO get current process working directory
		return (NULL);
	} else {
		curr_dentry = vfs_root_node->child;
		old_dentry = vfs_root_node;
		path = path + 1;
	}

	// VFS walk entry !
	while (path[0] != '\0')
	{
		// Get file name
		name_lenght = 0;
		while (name_lenght < 24 &&
				path[name_lenght] != '/' &&
				path[name_lenght] != '\0')
		{
			name[name_lenght] = path[name_lenght];
			name_lenght = name_lenght + 1;
		}

		// Check error
		if (path[name_lenght] != '/' && path[name_lenght] != '\0')
			return (NULL);

		// Add null char
		name[name_lenght] = '\0';

		// Check parthname or path
		if (mode == 1 && path[name_lenght] == '\0')
			return (old_dentry);

		// Check potential error
		if (curr_dentry == NULL)
			return (NULL);

		// Try to find file in the current VFS level.
		while (strncmp(curr_dentry->name, name, name_lenght) != 0)
		{
			curr_dentry = vfs_dentry_find_next_sibling(curr_dentry);
			if (curr_dentry == NULL)
				return (NULL);
		}

		// Directory check
		if (path[name_lenght] == '/')
		{
			// Check type
			if ((curr_dentry->mode & __S_IFDIR) == 0)
				return (NULL);

			// Try to find first child
			old_dentry = curr_dentry;
			curr_dentry = vfs_dentry_find_first_child(curr_dentry);

			// Update name lenght to skip '/' char
			name_lenght = name_lenght + 1; 
		}
	
		// Update path
		path = &path[name_lenght];
	}
	return (curr_dentry);
}
