/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tab.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: penzo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/24 11:21:14 by penzo             #+#    #+#             */
/*   Updated: 2019/06/24 11:21:18 by penzo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include "signals.h"
#include "history.h"
#include "line_editing.h"

static int	check_auto_complete(t_vars *vars, t_st_cmd *st_cmd,
		char **tmp, int mode)
{
	if (mode == regular
		&& (*tmp = auto_completion(st_cmd->st_txt->txt,
				st_cmd->st_txt->tracker, vars)))
		return (1);
	else if ((mode == heredoc || mode == continue_read)
		&& (*tmp = auto_completion_spe(st_cmd->st_txt->txt,
				st_cmd->st_txt->tracker)))
		return (1);
	return (0);
}

static void	refresh_prompt_after_completion(t_st_cmd *st_cmd, char **tmp,
	size_t len_tmp)
{
	ft_strdel(tmp);
	execute_str(BEGIN_LINE);
	execute_str(CLEAR_BELOW);
	print_prompt(st_cmd);
	st_cmd->st_txt->tracker = 0;
	write_st_cmd(st_cmd);
	st_cmd->st_txt->tracker = len_tmp;
}

int			check_for_tab(t_st_cmd *st_cmd, const char *buf,
	t_vars *vars, int mode)
{
	char	*tmp;
	char	*old_txt;
	size_t	len_tmp;

	len_tmp = 0;
	if (ft_strncmp(buf, "\t", 2) == 0 && isatty(TERM_FD))
	{
		old_txt = st_cmd->st_txt->txt;
		if (check_auto_complete(vars, st_cmd, &tmp, mode))
		{
			len_tmp = ft_strlen(tmp);
			if (!(st_cmd->st_txt->txt = ft_strjoin(tmp, st_cmd->st_txt->txt
						+ st_cmd->st_txt->tracker)))
				clean_exit(1, 1);
			st_cmd->st_txt->data_size = ft_strlen(st_cmd->st_txt->txt);
			st_cmd->st_txt->malloc_size = st_cmd->st_txt->data_size + 1;
			ft_strdel(&old_txt);
		}
		refresh_prompt_after_completion(st_cmd, &tmp, len_tmp);
		return (1);
	}
	else
		return (0);
}
