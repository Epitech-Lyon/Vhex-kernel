#include <command.h>
#include <string.h>

// External symbols defined by the linker script.
extern uint32_t bcmd_cache;
extern uint32_t ecmd_cache;

//
// cmd_cache_find()
// Try to find the command into the command cache.
//
// NOTE:
// The cache is generated during the compilation; all command
// blocks is stored in the ".cmd.cache" section.
// We use the section symbols to know where the cache end.
// (see bootstrap.ld).
//
const struct cmd_info *cmd_cache_find(char const *name)
{
	const struct cmd_info *cache;
	size_t i;
	
	i = -1;
	cache = (void *)&bcmd_cache;
	while ((ptrdiff_t)(&cache[++i]) != (ptrdiff_t)&ecmd_cache
	&& strcmp(cache[i].name, name));
	if ((ptrdiff_t)(&cache[i]) == (ptrdiff_t)&ecmd_cache)
		return (NULL);
	return (&cache[i]);
}
