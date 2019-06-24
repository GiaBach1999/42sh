/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   st_prompt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: penzo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/24 11:21:14 by penzo             #+#    #+#             */
/*   Updated: 2019/06/24 11:21:18 by penzo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_editing.h"

/*
**	Returns a freshly allocated st_prompt with an allocated copy of the prompt
**	parameter. If paramter is empty, duplicates the STD_PROMPT macro.
*/

t_st_prompt	*init_st_prompt(const char *prompt)
{
	t_st_prompt	*st_prompt;

	if (!(st_prompt = (t_st_prompt*)malloc(sizeof(*st_prompt))))
		clean_exit(1, 1);
	if (!prompt)
	{
		if (!(st_prompt->prompt = ft_strdup(STD_PROMPT)))
			clean_exit(1, 1);
	}
	else
		st_prompt->prompt = ft_strdup(prompt);
	st_prompt->size = ft_strlen(st_prompt->prompt);
	return (st_prompt);
}

/*
**	Memdels st_prompt and frees up the allocated memory
*/

void		free_st_prompt(t_st_prompt **st_prompt)
{
	if (!st_prompt || !(*st_prompt))
		return ;
	ft_strdel(&(*st_prompt)->prompt);
	ft_memdel((void*)st_prompt);
}
