/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_ulen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: penzo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/24 11:21:26 by penzo             #+#    #+#             */
/*   Updated: 2019/06/24 11:21:27 by penzo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		get_ulen(unsigned int value)
{
	int ulen;

	ulen = 0;
	while (value > 0)
	{
		ulen++;
		value /= 10;
	}
	return (ulen == 0 ? 1 : ulen);
}

int		get_ulllen(ULL value)
{
	int ulen;

	ulen = 0;
	while (value > 0)
	{
		ulen++;
		value /= 10;
	}
	return (ulen == 0 ? 1 : ulen);
}

int		get_ullen(UL value)
{
	int ulen;

	ulen = 0;
	while (value > 0)
	{
		ulen++;
		value /= 10;
	}
	return (ulen == 0 ? 1 : ulen);
}

int		ulen(t_arg *specs, ULL n)
{
	if (specs->l == 2)
		return (get_ulllen((ULL)n));
	if (specs->l == 1)
		return (get_ullen((UL)n));
	if (specs->h == 2)
		return (get_ulen((unsigned char)n));
	if (specs->h == 1)
		return (get_ulen((unsigned short int)n));
	else
		return (get_ulen((unsigned int)n));
}
