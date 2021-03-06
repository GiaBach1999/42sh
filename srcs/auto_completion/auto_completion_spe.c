#include "errors.h"
#include "auto_completion.h"
#include "input.h"

static int			get_all(const char *directory,
		t_auto_comp **match)
{
	DIR				*dir;
	struct dirent	*ent;
	char			*tmp;

	if ((dir = opendir(directory)) == NULL)
		return (0);
	while ((ent = readdir(dir)))
	{
		tmp = NULL;
		if (ft_strncmp("..", ent->d_name, 3) && ft_strncmp(".", ent->d_name, 2))
		{
			if (check_if_slash(ent, directory, NULL))
				tmp = ft_strjoin(ent->d_name, "/");
			else
				tmp = ft_strjoin(ent->d_name, " ");
			if (!tmp)
				clean_exit(1, MALLOC_ERR);
			create_match_link(match, tmp);
			ft_strdel(&tmp);
		}
	}
	if (closedir(dir) == -1)
		return (1);
	return (0);
}

static int			special_case(char **r_str, char *input, char *next)
{
	if (input && input[0] == '$')
	{
		*r_str = varz(input, next);
		return (1);
	}
	if (input && input[0] == '~' && (!input[1] || input[1] != '/'))
	{
		*r_str = users_passwd(input);
		return (1);
	}
	return (0);
}

static char			*get_match_and_display_hdoc(char *input,
					char *next)
{
	char			*path;
	char			*to_f;
	char			*tmp;
	char			*r_str;
	t_auto_comp		*match;

	initialize_str(&to_f, &path, &r_str, &tmp);
	match = NULL;
	if ((special_case(&r_str, input, next)))
		return (r_str);
	if (input && input[0] && ft_strchr(input, '/'))
		tmp = ft_strndup(input, ft_strlen(input)
				- ft_strlen(ft_strrchr(input, '/') + 1));
	get_path_file_and_to_find((char *)input, &path, &to_f);
	if (!to_f[0])
		get_all(path, &match);
	get_all_match(path, &match, to_f, next);
	if (match)
		r_str = get_ret_or_display_matches(match, to_f, ft_strlen(to_f), 0);
	else if (!(r_str = ft_strdup(to_f)))
		clean_exit(1, MALLOC_ERR);
	get_good_ret_str(&r_str, tmp);
	free_four_strings(&tmp, NULL, &path, &to_f);
	return (r_str);
}

char				*auto_completion_spe(char *input, unsigned int len)
{
	char			*to_find_full;
	char			*ret;
	char			*tmp;
	char			*str;
	int				start_actual_word;

	ret = NULL;
	tmp = NULL;
	if (!input)
		return (NULL);
	start_actual_word = get_needed_values(input, len, &str, &to_find_full);
	ret = get_match_and_display_hdoc(to_find_full + start_actual_word,
			str + start_actual_word);
	format_finding_and_get_correct_ret(&ret,
			start_actual_word + (len - ft_strlen(to_find_full)), input, len);
	free_two_strings(&to_find_full, &str);
	return (ret);
}
