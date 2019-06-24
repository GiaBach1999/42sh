/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: penzo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/24 11:21:14 by penzo             #+#    #+#             */
/*   Updated: 2019/06/24 11:21:18 by penzo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include "errors.h"

/*
**	Returns a freshly allocated string containing the value of value associated
**	with the env_key paramter. If env_key is not found in env, returns NULL.
*/

char			*get_directory(const char *env_key, const char **env)
{
	char			*dest;

	dest = NULL;
	if (!(dest = get_envline_value((char*)env_key, (char**)env)))
	{
		if (ft_strncmp(env_key, "PWD", 4))
			print_errors(ERR_NOT_SET, ERR_NOT_SET_STR, (char*)env_key);
		return (NULL);
	}
	if (!(dest = ft_strdup(dest)))
		clean_exit(1, 1);
	return (dest);
}

static char		*get_cwd_value(const char **env, char opt)
{
	char			*cwd;

	cwd = NULL;
	if (opt != 'P' && (cwd = get_envline_value("PWD", (char**)env)))
	{
		if (!(cwd = ft_strdup(cwd)))
			clean_exit(1, 1);
	}
	else if (!(cwd = getcwd(NULL, 0)))
	{
		print_errors(ERR_GETCWD, ERR_GETCWD_STR, NULL);
		return (NULL);
	}
	return (cwd);
}

/*
**	Get the right path with getcwd call, for easier chdir execution
*/

char			*relative_directory(const char *path, const char **env,
		int *cdpath, char opt)
{
	char			*cwd;
	char			*tmp;
	char			*dest;

	ft_initialize_str(&cwd, &tmp, &dest, NULL);
	if (!(cwd = get_cwd_value(env, opt)))
		return (NULL);
	if (check_cdpath_var(path, env, &tmp, cdpath))
	{
		if (!(tmp = ft_strjoin(cwd, "/")))
			clean_exit(1, 1);
		ft_strdel(&cwd);
	}
	if (!(dest = ft_strjoin(tmp, path)))
		clean_exit(1, 1);
	if (tmp != cwd)
		ft_strdel(&tmp);
	ft_strdel(&cwd);
	return (dest);
}
