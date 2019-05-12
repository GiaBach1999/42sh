#include "42sh.h"
#include "errors.h"

static int		check_access(char *file)
{
	if (access(file, F_OK) == 0)
	{
		if (access(file, X_OK) == 0)
			return (0);
		return (ERR_ACCESS);
	}
	return (ERR_NOEXIST);
}

static char		*find_path(char *file, char **paths)
{
	unsigned int	i;
	char			*possible_path;
	char			*path_w_slash;
	
	i = 0;
	while (paths[i])
	{
		if (!(path_w_slash = ft_strjoin(paths[i], "/")))
			ERROR_MEM;
		if (!(possible_path = ft_strjoin(path_w_slash, file)))
			ERROR_MEM;
		ft_strdel(&path_w_slash);
		if (check_access(possible_path) == 0)
			return (possible_path);
		ft_strdel(&possible_path);
		i++;
	}
	return (NULL);
}

char	*get_cmd_path(char **argv, char **env)
{
	char	**paths;
	char	*path_line;
	char	*path;
	int		acc;

	path = NULL;
	if (ft_strchr(argv[0], '/'))
		path = argv[0];
	else if (!(path_line = get_envline_value("PATH", env)))
		path = argv[0];
	else
	{
		if (!(paths = ft_strsplit(path_line, ":")))
			ERROR_MEM;
	}
	if (!path && !(path = find_path(argv[0], paths)))
		path = argv[0];
	acc = check_access(path);
	if (acc == 0)
		return (path);
	else 
	{
		if (acc == ERR_NOEXIST)
			print_errors(ERR_NOEXIST, ERR_NOEXIST_STR, path);
		else if (acc== ERR_ACCESS)
			print_errors(ERR_ACCESS, ERR_ACCESS_STR, path);
		return (NULL);
	}
}