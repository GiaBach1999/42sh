/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insertion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: penzo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/24 11:21:14 by penzo             #+#    #+#             */
/*   Updated: 2019/06/24 11:21:18 by penzo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftsh.h"
#include "history.h"

t_hist_lst		*insert_left(t_hist_lst *hist_lst, const char *line, char keep)
{
	t_hist_lst	*probe;
	t_hist_lst	*insert;

	if (!(insert = create_hist_lst(line, keep)))
		return (NULL);
	if (!(probe = hist_lst))
		return (insert);
	else
	{
		if (probe->prev)
		{
			probe->prev->next = insert;
			insert->prev = probe->prev;
		}
		insert->next = probe;
		probe->prev = insert;
	}
	return (insert);
}

t_hist_lst		*insert_right(t_hist_lst *hist_lst, const char *line, char keep)
{
	t_hist_lst	*probe;
	t_hist_lst	*insert;

	if (!(insert = create_hist_lst(line, keep)))
		return (NULL);
	if (!(probe = hist_lst))
		return (insert);
	else
	{
		if (probe->next)
		{
			probe->next->prev = insert;
			insert->next = probe->next;
		}
		probe->next = insert;
		insert->prev = probe;
	}
	return (insert);
}
