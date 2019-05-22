#include "libft.h"
#include "line_editing.h"

int					get_everything_from_path(char *path, t_auto_comp **match)
{
	DIR				*dir;
	struct dirent	*ent;
	char			*tmp;

	if ((dir = opendir(path)) == NULL)
		return (1);
	while ((ent = readdir(dir)))
	{
		if (ft_strncmp(ent->d_name, "..", 2) && ft_strncmp(ent->d_name, ".", 1))
		{
			tmp = ft_strdup(ent->d_name);
			create_match_link(match, tmp);
			ft_strdel(&tmp);
		}
	}
	if (closedir(dir) == -1)
		return(1);
	return (0);

}

int					find_exe_space(char **path, t_auto_comp **match)
{
	int				i;
	char			*true_path;

	true_path = NULL;
	i = 0;
	while (path[i])
	{
		true_path = rm_spaces_path(path[i++]);
		get_everything_from_path(true_path, match);
		ft_strdel(&true_path);
	}
	return (0);
}

int					find_all_var_and_exe(char **path, t_auto_comp **match)//tous static?
{
	if (find_exe_space(path, match))
		ERROR_MEM
			/*if (find_alias_space(match)
			 * ERROR_MEM
			 * if (find_reserved_words_space(match)
			 * ERROR_MEM*/
			return (0);
}

char				*new_auto_completion_space(t_vars *vars)
{
	char			**path;
	t_auto_comp		*match;

	path = NULL;
	match = NULL;
	if (get_path(&path, vars))
		ERROR_MEM
	if (find_all_var_and_exe(path, &match))
		ERROR_MEM
	if (path)
		ft_free_ntab(path);
	if (match)
		get_ret_or_display_matches(match, NULL, 0);
	return (NULL);
	}
