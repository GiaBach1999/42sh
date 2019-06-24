/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_doc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: penzo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/24 11:21:17 by penzo             #+#    #+#             */
/*   Updated: 2019/06/24 11:21:21 by penzo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include "heredoc.h"
#include "history.h"
#include "cmd_parsing.h"

static void		apply_escape(t_st_cmd *st_cmd)
{
	int				i;
	unsigned char	is_real_escape;

	i = ft_strlen(st_cmd->st_txt->txt) - 1;
	is_real_escape = 0;
	while (--i >= 0 && st_cmd->st_txt->txt[i] == '\\')
	{
		if (st_cmd->st_txt->txt[i] == '\\')
			is_real_escape = (is_real_escape) ? 0 : 1;
	}
	if (is_real_escape)
	{
		st_cmd->st_txt->txt[st_cmd->st_txt->data_size - 1] = 0;
		st_cmd->st_txt->txt[st_cmd->st_txt->data_size - 2] = 0;
		st_cmd->st_txt->data_size -= 2;
	}
}

static char		*get_heredoc_txt(char *txt, char *eof)
{
	char	*trimed_txt;
	size_t	len;

	len = ft_strlen(txt) - ft_strlen(eof) - 2;
	if (!(trimed_txt = ft_strndup(&txt[1], len)))
		clean_exit(1, 1);
	ft_strdel(&txt);
	ft_strdel(&eof);
	return (trimed_txt);
}

static char		*return_get_doc(char *txt, unsigned char is_eof_quoted,
			t_vars *vars)
{
	char	*path;

	if (!is_eof_quoted && (parse_dollars_str(&txt, vars) > 0))
	{
		ft_strdel(&txt);
		return (NULL);
	}
	if (!(path = write_heredoc_in_file(&txt)))
		return (NULL);
	return (path);
}

static void		init_get_doc(t_st_cmd **cmd, char **txt, t_st_cmd **heredoc)
{
	*cmd = get_last_st_cmd(get_st_cmd(NULL));
	*txt = NULL;
	*cmd = append_st_cmd(*cmd, "", HEREDOC_PROMPT);
	*heredoc = *cmd;
}

/*
** get_doc
** read the input from user until a line contain only 'eof' string
** write the input into a temporary file
** and return the path to it
*/

char			*get_doc(char *eof, unsigned char is_eof_quoted, t_vars *vars)
{
	char		*txt;
	t_st_cmd	*cmd;
	t_st_cmd	*start_heredoc;
	int			len;

	init_get_doc(&cmd, &txt, &start_heredoc);
	while (42)
	{
		if ((len = input_loop(cmd, vars, heredoc)) < 1 || !*cmd->st_txt->txt)
			return (free_get_doc(txt, eof, cmd, start_heredoc));
		if (!is_eof_quoted)
			apply_escape(cmd);
		txt = concatenate_heredoc_txt(cmd, start_heredoc);
		len = ft_strlen(txt) - ft_strlen(eof) - 1;
		if (len > 0 && !ft_strncmp(&txt[len], eof, ft_strlen(eof))
			&& txt[len - 1] == '\n' && txt[ft_strlen(txt) - 1] == '\n')
			break ;
		ft_strdel(&txt);
		cmd = append_st_cmd(cmd, "", HEREDOC_PROMPT);
	}
	txt = get_heredoc_txt(txt, eof);
	clean_heredoc(cmd, start_heredoc);
	return (return_get_doc(txt, is_eof_quoted, vars));
}
