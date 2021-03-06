#include "errors.h"
#include "ftsh.h"
#include "builtins.h"
#include "execution.h"

int					case_fc_display(t_st_cmd *st_cmd, t_st_fc *st_fc)
{
	int				i_curr;
	t_hist_lst		*hist_curr;

	if (ft_strchr(st_fc->flag, 'r'))
		return (fc_display_reverse(st_cmd, st_fc));
	i_curr = st_fc->i_first;
	hist_curr = get_entry_lst(st_cmd->hist_lst, i_curr);
	while (i_curr <= st_fc->i_last && hist_curr)
	{
		if (!ft_strchr(st_fc->flag, 'n'))
			ft_putnbr(i_curr);
		ft_dprintf(1, "\t%s", hist_curr->txt);
		i_curr++;
		hist_curr = hist_curr->next;
	}
	return (0);
}

char				*substitute_pattern(t_st_fc *st_fc, char *histo_real_entry,
		int old_cmd_len)
{
	char			*new_cmd;

	new_cmd = NULL;
	if ((!st_fc->old_pattern || !st_fc->old_pattern[0])
		&& (!st_fc->new_pattern || !st_fc->new_pattern[0]))
	{
		if (!(new_cmd = ft_strdup(histo_real_entry)))
			clean_exit(1, MALLOC_ERR);
		return (new_cmd);
	}
	else if (!st_fc->old_pattern[0] && st_fc->new_pattern[0])
		new_cmd = fc_s_no_old_yes_new(st_fc, old_cmd_len);
	else if (st_fc->old_pattern[0] && !st_fc->new_pattern[0])
		new_cmd = fc_s_yes_old_no_new(st_fc, histo_real_entry, old_cmd_len);
	else if (st_fc->old_pattern && st_fc->new_pattern)
		new_cmd = fc_s_yes_old_yes_new(st_fc, histo_real_entry, old_cmd_len);
	return (new_cmd);
}

static int			case_fc_substitute(t_st_cmd *st_cmd, t_st_fc *st_fc)
{
	char			*new_cmd;
	int				old_cmd_len;
	t_hist_lst		*hist_curr;
	int				diff;
	char			*histo_real_entry;

	new_cmd = NULL;
	diff = *st_cmd->hist_len - st_fc->i_first + 1;
	if (!st_cmd->hist_lst)
		return (-1);
	hist_curr = st_cmd->hist_lst;
	while (hist_curr->prev && diff--)
		hist_curr = hist_curr->prev;
	old_cmd_len = ft_strlen_char(hist_curr->txt, '\n');
	histo_real_entry = ft_strndup(hist_curr->txt, old_cmd_len);
	new_cmd = substitute_pattern(st_fc, histo_real_entry, old_cmd_len);
	ft_strdel(&histo_real_entry);
	if (new_cmd && new_cmd[0])
	{
		diff = fc_execute_cmd(st_cmd, new_cmd, substitute);
		ft_strdel(&new_cmd);
		return (diff);
	}
	return (0);
}

static int			case_fc_editor(t_st_cmd *st_cmd, t_st_fc *st_fc)
{
	int				tmp_file_fd;
	char			*tmp_file;
	int				ret;

	if ((tmp_file_fd = fc_edit_open_file(st_cmd, st_fc, &tmp_file)) == -1)
		return (1);
	if ((fc_edit_open_editor(st_cmd, st_fc, &tmp_file, tmp_file_fd)) > 0)
	{
		ft_strdel(&tmp_file);
		return (1);
	}
	ret = fc_execute_cmd(st_cmd, tmp_file, edit);
	ft_strdel(&tmp_file);
	return (ret);
}

/*
**	Init struct_fc depending on options, then execute the adequate cqse.
**	Returns 0 on success.
**	Returns 1 on failure.
*/

int					case_fc(char **argv)
{
	t_st_fc			st_fc;
	t_st_cmd		*st_cmd;
	int				ret;

	ft_bzero(&st_fc, sizeof(st_fc));
	st_cmd = get_st_cmd(NULL);
	st_cmd->keep = 0;
	if ((ret = init_st_fc(st_cmd, &st_fc, argv)) == 1 || ret == 2)
	{
		free_st_fc(&st_fc);
		st_cmd->hist_lst = get_end_lst(st_cmd->hist_lst);
		return (ret);
	}
	if (st_fc.flag[0] == 's')
		ret = case_fc_substitute(st_cmd, &st_fc);
	else if (ft_strchr(st_fc.flag, 'l'))
	{
		st_cmd->keep = 1;
		ret = case_fc_display(st_cmd, &st_fc);
	}
	else
		ret = case_fc_editor(st_cmd, &st_fc);
	free_st_fc(&st_fc);
	return (ret);
}
