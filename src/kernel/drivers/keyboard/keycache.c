#include <kernel/drivers/keyboard.h>
#include <kernel/util/atomic.h>

/* keycache_alloc() - Try to alloc a new keycache node */
static struct keycache_s *keycache_alloc(void)
{
	extern struct keycache_s keycache[96];
	void *node;
	int i;

	// Start atomic operations
	atomic_start();

	// Try to find free slot
	i = 96;
	node = NULL;
	while (--i >= 0)
	{
		if (keycache[i].keycode == KEY_UNUSED)
		{
			node = &keycache[i];
			break;
		}
	}

	// Stop atomic operations
	atomic_stop();
	return (node);
}

/* keycache_update() - Add / update key code node */
void keycache_update(int row, int column, uint8_t key_frame)
{
	extern struct keycache_s *keylist;
	struct keycache_s *current_node;
	struct keycache_s *new_node;
	uint8_t keycode;

	// Generate keycode.
	keycode = KEYCODE_GEN(row, column);

	// Start atomic operations
	atomic_start();

	// Try to fint the key node.
	current_node = keylist;
	while (current_node != NULL)
	{
		// If key node is found, update key frame and exit.
		if (current_node->keycode == keycode)
		{
			current_node->key_frame = key_frame;
			atomic_stop();
			return;
		}

		// Get next node and retry.
		current_node = current_node->next;
	}

	// If no node is found, create new node.
	new_node = keycache_alloc();
	if (new_node != NULL)
	{
		// Fill new node
		new_node->keycode = keycode;
		new_node->key_frame = key_frame;
		new_node->counter = 0;

		// And place it on the first node because
		// the first node designates the last key
		// pressed.
		new_node->next = keylist;
		keylist = new_node;
	}

	// Stop atomic operations
	atomic_stop();
}

/* keycache_clean() - Remove dirty node */
void keycache_clean(int key_frame)
{
	extern struct keycache_s *keylist;
	struct keycache_s **current_node;
	
	// Start atomic operations
	atomic_start();

	current_node = &keylist;
	while (*current_node != NULL)
	{
		// Check if the key has been released,
		// remove the key node.
		if ((*current_node)->key_frame != key_frame)
		{
			(*current_node)->keycode = KEY_UNUSED;
			*current_node = (*current_node)->next;
			continue;
		}

		// If the key is press, update internal
		// counter and get nex key node.
		(*current_node)->counter = (*current_node)->counter + 1;
		current_node = &(*current_node)->next;
	}

	// Stop atomic operations
	atomic_stop();
}
