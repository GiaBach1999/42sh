#include "ftsh.h"
#include "history.h"

void			free_st_fc(t_st_fc *st_fc)
{
	if (st_fc->first)
		free(st_fc->first);
	if (st_fc->last)
		free(st_fc->last);
	if (st_fc->editor)
		free(st_fc->editor);
	if (st_fc->old_pattern)
		free(st_fc->old_pattern);
	if (st_fc->new_pattern)
		free(st_fc->new_pattern);
}

static void		print_usage_fc(void)
{
	int			fd;

	fd = STDERR_FILENO;
	ft_dprintf(fd, "fc: usage:\n");
	ft_dprintf(fd, "    fc [-r] [-e editor] [first [last]]\n");
	ft_dprintf(fd, "OR  fc -l [-nr] [first [last]]\n");
	ft_dprintf(fd, "OR  fc -s [old=new] [first]\n");
}

int				error_fc(char *s, int i, int type, t_st_fc *st_fc)
{
	(void)i;
	(void)st_fc;
	ft_dprintf(STDERR_FILENO, "%s: fc: ", SHELL_NAME);
	if (type == path_unspecified)
		ft_dprintf(2, "-e: $PATH unset: impossible to find editor '%s'\n", s);
	else if (type == cmd_not_found)
		ft_dprintf(2, "%s: command not found\n", s);
	print_usage_fc();
	return (-1);
}

int				error_fc_index(char *flag)
{
	t_st_cmd	*st_cmd;

	st_cmd = get_st_cmd(NULL);
	ft_dprintf(STDERR_FILENO, "%s: fc: ", SHELL_NAME);
	if (flag[0] == 's')
	{
		ft_dprintf(2, "no command found\n");
	}
	else if (!flag || flag[0] == '.'
		|| (flag && (ft_strchr(flag, 'l') || ft_strchr(flag, 'e'))))
	{
		ft_dprintf(2, "history specification out of range\n");
		if (!ft_strchr(flag, 'e') && flag[0] != '.')
			st_cmd->keep = 1;
	}
	return (-1);
}

int				error_fc_histo(char *s, int i, int type, t_st_fc *st_fc)
{
	t_st_cmd	*st_cmd;
	int			len;

	st_cmd = get_st_cmd(NULL);
	st_cmd->keep = 1;
	ft_dprintf(STDERR_FILENO, "%s: fc: ", SHELL_NAME);
	if (type == invalid_option)
	{
		ft_dprintf(STDERR_FILENO, "-%c: invalid option\n", s[i]);
		print_usage_fc();
		return (-2);
	}
	else if (type == invalid_mix)
	{
		len = ft_strlen_char(st_fc->flag, '.');
		st_fc->flag[len] = s[i];
		ft_putchar('-');
		write(STDERR_FILENO, st_fc->flag, len + 1);
		ft_dprintf(STDERR_FILENO, ": invalid option mix\n");
		print_usage_fc();
		return (-2);
	}
	else if (type == editor_unspecified)
		ft_dprintf(2, "-e: option requires an argument\n");
	return (-1);
}
