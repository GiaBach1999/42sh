/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_history.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: penzo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/24 11:21:14 by penzo             #+#    #+#             */
/*   Updated: 2019/06/24 11:21:18 by penzo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

static t_hist_lst	*get_hist_size_start(t_hist_lst *hist_lst)
{
	t_hist_lst		*res;
	unsigned int	i;

	res = get_end_lst(hist_lst);
	if (!res)
		return (res);
	i = 0;
	while (i < HIST_SIZE && res->prev)
	{
		res = res->prev;
		i++;
	}
	return (res);
}

static void			actual_writing_to_history(t_hist_lst *hist_lst,
					size_t id, int fd)
{
	int				i;

	i = 0;
	ft_dprintf(fd, "%4d  ", id);
	while (hist_lst->txt[i])
	{
		write(fd, &hist_lst->txt[i], 1);
		if (hist_lst->txt[i] == '\n' && hist_lst->txt[i + 1] != 0)
		{
			id++;
			ft_dprintf(fd, "%4d  ", id);
		}
		i++;
	}
}

int					write_to_history(t_st_cmd *st_cmd, const char **env)
{
	t_hist_lst		*hist_lst;
	size_t			id;
	int				fd;

	if (isatty(TERM_FD) == 0 || !st_cmd || !(st_cmd->hist_lst))
		return (0);
	if ((fd = open_history(env, O_WRONLY | O_CREAT | O_TRUNC)) < 0)
		return (0);
	else
	{
		hist_lst = get_hist_size_start(st_cmd->hist_lst);
		id = 0;
		while (hist_lst)
		{
			if (hist_lst->keep)
			{
				actual_writing_to_history(hist_lst, id, fd);
				id++;
			}
			hist_lst = hist_lst->next;
		}
		close(fd);
		return (1);
	}
}
