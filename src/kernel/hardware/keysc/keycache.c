#include <kernel/devices/keyboard.h>

// Internal cache, used like chained list.
// @note:
// 	The KEYSC have 6 key data 16-bits registers
// this is why we used 6 * 16 = 96 cache slot.
static struct keycache_s keycache[96];

// chained list.
// FIXME: avoid reentrace 
struct keycache_s *keylist;

static struct keycache_s *keycache_alloc(void)
{
	int i;

	i = 96;
	while (--i >= 0)
	{
		if (keycache[i].keycode == KEY_UNUSED)
			return (&keycache[i]);
	}
	return (NULL);
}

void keycache_update(int row, int column, uint8_t key_frame)
{
	struct keycache_s *current_node;
	struct keycache_s *new_node;
	uint8_t keycode;

	// Generate keycode.
	keycode = KEYCODE_GEN(row, column);

	// Try to fint the key node.
	current_node = keylist;
	while (current_node != NULL)
	{
		// If key node is found, update key frame and exit.
		if (current_node->keycode == keycode)
		{
			current_node->key_frame = key_frame;
			return;
		}

		// Get next node and retry.
		current_node = current_node->next;
	}

	// If no node is found, create new node.
	new_node = keycache_alloc();
	if (new_node == NULL)
		return;

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

void keycache_clean(int key_frame)
{
	struct keycache_s **current_node;
	
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
}

// Contructor.
void keycache_init(void)
{
	int i;

	i = 96;
	while (--i >= 0)
	{
		keycache[i].keycode = KEY_UNUSED;
		keycache[i].next = NULL;
	}
	keylist = NULL;
}
