/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   st_cmd_getters.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: penzo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/24 11:21:14 by penzo             #+#    #+#             */
/*   Updated: 2019/06/24 11:21:18 by penzo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_editing.h"

/*
**	Singleton function to set or retrieve st_cmd address
*/

t_st_cmd	*get_st_cmd(t_st_cmd **new_struct)
{
	static t_st_cmd *st_cmd = NULL;

	if (new_struct)
		st_cmd = *new_struct;
	return (st_cmd);
}

t_st_cmd	*get_first_st_cmd(t_st_cmd *st_cmd)
{
	if (st_cmd)
		while (st_cmd->prev)
			st_cmd = st_cmd->prev;
	return (st_cmd);
}

t_st_cmd	*get_last_st_cmd(t_st_cmd *st_cmd)
{
	if (st_cmd)
		while (st_cmd->next)
			st_cmd = st_cmd->next;
	return (st_cmd);
}
