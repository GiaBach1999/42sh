/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: penzo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/24 11:21:16 by penzo             #+#    #+#             */
/*   Updated: 2019/06/24 11:21:20 by penzo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftsh.h"
#include "exp_arith.h"

/*
**	Second tokenization : removing variable iterations, and '+' and '-' which
**	are not operators but sign marks, and building a clean token array
*/

static	int		format_toks(t_tok *di, t_tok **cl)
{
	t_integ	v;

	v.prev = 0;
	v.moins = 1;
	v.i = 0;
	v.k = 0;
	init_toks_clean(di, cl);
	while (di[v.i].token != 0)
	{
		if (di[v.i].token == tk_nb)
			c_number(di, cl, &v);
		else if (di[v.i].token <= tk_minvar && di[v.i].token >= tk_varplus)
			c_iter(di, cl, (int*)&v.i, (int*)&v.k);
		else
			c_operator(di, cl, &v);
		v.i++;
	}
	return (0);
}

/*
**	First tokenization : almost raw interpretation of the text and operators,
**	but the iterators and the variables are identified, and building a
**	dirty token array
*/

int				op_tokenizer(char *str, t_tok **tokens, char ***vars)
{
	t_integ		ints;
	t_tok		*clean;

	ints.k = 0;
	ints.i = 0;
	init_toks_dirty(tokens, str);
	while (!ft_str_isempty(str))
	{
		while (ft_is_white_space(str[ints.i]))
			ints.i++;
		if (get_nb_len(str + ints.i))
			d_number(tokens, &ints.i, &ints.k, &str);
		else if (is_var(str + ints.i))
			d_variable(tokens, &ints, &str, vars);
		else
			d_operator(tokens, &ints.i, &ints.k, &str);
		ints.k++;
	}
	format_toks(*tokens, &clean);
	free_tokens(*tokens, 0);
	*tokens = clean;
	return (0);
}
