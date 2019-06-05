#include "input.h"
#include "signals.h"
#include "history.h"
#include "line_editing.h"
#include "builtins.h"
#include "libft.h"

int		strstr_adapted(const char *haystack, const char *needle)
{
	size_t			i;
	size_t			j;

	if (*needle == 0)
		return (0);
	i = 0;
	while (haystack[i])
	{
		j = 0;
		while (haystack[i] == needle[j] && needle[j])
		{
			i++;
			j++;
		}
		if (needle[j] == 0)
			return (1);
		i = i - j + 1;
	}
	return (0);
}

int		init_vars_reverse_search_histo
		(size_t *malloc_size, int *prompt_type, char **stock)
{
	*malloc_size = 256;
	*prompt_type = 0;
	if (!(*stock = ft_strnew(*malloc_size + 1)))
		ERROR_MEM;
	return (0);
}

void	realloc_stock(char **stock, char buf, size_t malloc_size)
{
	(*stock) = ft_realloc((*stock), ft_strlen((*stock)) - 1, &malloc_size, 1);
	(*stock)[ft_strlen((*stock))] = buf;
}

int		is_quit_char(char buf)
{
	if (buf != 18  && buf != ' '
		&& ft_isprint(buf) == 0)
		return (1);
	return (0);
}
