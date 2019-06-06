#include "ftsh.h"

char				*get_var_name(char *str)
{
	size_t		i;
	char		*var_name;

	str++;
	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	if (i == 0)
		return (NULL);
	if (!(var_name = ft_strndup(str, i)))
		ERROR_MEM;
	return (var_name);
}
