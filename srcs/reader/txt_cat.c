/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   txt_cat.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: penzo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/24 11:21:14 by penzo             #+#    #+#             */
/*   Updated: 2019/06/24 11:21:18 by penzo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

/*
**	Returns a string containing all string contents in the st_cmd list.
*/

char	*concatenate_txt(t_st_cmd *st_cmd)
{
	char	*input;

	if (!st_cmd)
		return (NULL);
	st_cmd = get_first_st_cmd(st_cmd);
	if (!(input = ft_strdup("")))
		clean_exit(1, 1);
	while (st_cmd)
	{
		if (!(input = ft_strjoin_free_left(input, st_cmd->st_txt->txt)))
			clean_exit(1, 1);
		st_cmd = st_cmd->next;
	}
	return (input);
}

char	*concatenate_heredoc_txt(t_st_cmd *st_cmd, t_st_cmd *start)
{
	char	*input;

	if (!st_cmd)
		return (NULL);
	st_cmd = get_first_st_cmd(st_cmd);
	while (st_cmd != start)
		st_cmd = st_cmd->next;
	if (!(input = ft_strdup("\n")))
		clean_exit(1, 1);
	while (st_cmd)
	{
		if (!(input = ft_strjoin_free_left(input, st_cmd->st_txt->txt)))
			clean_exit(1, 1);
		st_cmd = st_cmd->next;
	}
	return (input);
}
