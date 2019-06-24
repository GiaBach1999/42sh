/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_arith_exp.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: penzo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/24 11:21:14 by penzo             #+#    #+#             */
/*   Updated: 2019/06/24 11:21:18 by penzo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_parsing.h"
#include "exp_arith.h"

static char		*dup_for_arith_exp(char *str, size_t i, size_t index)
{
	char	*new_str;

	if (!(new_str = ft_strndup(str + index + 3, i - (index + 3))))
		clean_exit(1, 1);
	return (new_str);
}

static void		set_arith_exp_variables(size_t *var1_name, size_t var1_value,
		long long *var2_name, long long var2_value)
{
	*var1_name = var1_value;
	*var2_name = var2_value;
}

static char		expand_lowest_arith_exp(char **str, t_vars *vars)
{
	size_t		i;
	size_t		index[2];
	long long	arith_result;
	char		*arith_str;

	set_arith_exp_variables(&i, 0, &arith_result, 0);
	ft_bzero(index, sizeof(index));
	while ((*str)[i])
	{
		if (!(ft_strncmp("$((", *str + i, 3)))
			index[0] = i;
		if (!(ft_strncmp("))", *str + i, 2)))
		{
			index[1] = i + 1;
			arith_str = dup_for_arith_exp(*str, i, index[0]);
			if (expansion_arith(arith_str, &arith_result, vars))
				return (ft_free_str_return_char(&arith_str, -1));
			arith_str = lltoa_and_free(arith_result, &arith_str);
			substitute_slice(str, index, arith_str);
			ft_strdel(&arith_str);
			return (1);
		}
		i++;
	}
	return (0);
}

int				parse_arith_exp(char **str, t_vars *vars)
{
	size_t		i;
	char		escaped;
	char		arith_expand_ret;

	i = 0;
	escaped = 0;
	while (vars->verbose && (*str)[i])
	{
		if (!escaped && !ft_strncmp("$((", (*str) + i, 3)
				&& is_matched((*str) + i, "$((", "))"))
		{
			while ((arith_expand_ret = expand_lowest_arith_exp(str, vars)))
			{
				if (arith_expand_ret == -1)
					return (1);
			}
		}
		else if ((*str)[i] == '\\')
			escaped = (escaped) ? 0 : 1;
		else
			escaped = 0;
		i++;
	}
	return (0);
}
