#include "ftsh.h"
#include "builtins.h"
#include "env.h"

static int	varline_case(char *argv, t_vars *vars)
{
	if (argv[0] == '=')
		return (1);
	add_varline(argv, &vars->env_vars);
	add_varline(argv, &vars->shell_vars);
	return (0);
}

int			case_export(char **argv, t_vars *vars)
{
	int		i;
	char	*value;
	int		ret;

	i = 0;
	ret = 0;
	while (argv[++i])
	{
		value = NULL;
		if (ft_strchr(argv[i], '=') && varline_case(argv[i], vars))
			ret = 1;
		else if ((value = get_varline_value(argv[i], vars->shell_vars)))
			add_variables(argv[i], value, &vars->env_vars);
		else if ((value = get_varline_value(argv[i], vars->env_vars)))//this might be useless, but i keep it for safety
			add_variables(argv[i], value, &vars->shell_vars);
		if (value)
			ft_strdel(&value);
	}
	return (ret);
}
