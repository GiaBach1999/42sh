#include "lexer.h"
#include "line_editing.h"
#include "ast.h"
#include "history.h"
#include "cmd_parsing.h"
#include "execution.h"

static int		continue_reading(t_token *token_head, t_st_cmd **st_cmd,
				char **input, t_vars *vars)
{
	free_token_list(token_head);
	adjust_history(*st_cmd, 0);
	*st_cmd = append_st_cmd(*st_cmd, "",
		init_st_prompt(CONTINUE_PROMPT, NULL, 0));
	if (input_loop(*st_cmd, vars, continue_read) < 1)
	{
		ft_strdel(input);
		if (*(*st_cmd)->st_txt->txt != '\x03')
		{
			ft_dprintf(2, SHELL_NAME ": unexpected EOF\n");
			return (2);
		}
		else
			return (130);
	}
	ft_strdel(input);
	*input = concatenate_txt(*st_cmd, 1);
	if (is_full_of_whitespaces(*input))
	{
		ft_strdel(input);
		return (0);
	}
	return (1);
}

static int		hdl_cntread(t_token *token_head, t_st_cmd **st_cmd,
				char **input, t_vars *vars)
{
	int	ret;

	(*st_cmd)->cr = 1;
	ret = continue_reading(token_head, st_cmd, input, vars);
	if (!isatty(TERM_FD))
		ft_dprintf(2, "%s: unexpected end of file\n", SHELL_NAME);
	return (ret);
}

static int		handle_execution(t_st_cmd *st_cmd, t_token *token_head,
	t_vars *vars)
{
	int			ret;
	t_ast		*ast_root;

	ast_root = create_ast(token_head);
	ret = exec_ast(ast_root, vars, 1);
	if (st_cmd->keep)
		adjust_history(st_cmd, 1);
	free_ast(ast_root);
	return (exit_status(ret));
}

/*
** handle_input
** 1. get token_list from input
** 2. store input to history
** 3. get ast from token_list
** 4. execute ast
** 5. free ast
*/

int				handle_input(t_st_cmd *st_cmd, t_vars *vars)
{
	t_token			*tk;
	int				lexer_ret;
	int				ret;
	char			*input;

	tk = NULL;
	if (!(input = concatenate_txt(st_cmd, 1)))
		return (1);
	if (is_full_of_whitespaces(input))
	{
		ft_strdel(&input);
		return (0);
	}
	while ((lexer_ret = lexer(input, &tk, vars)) == lex_cont_read)
		if ((ret = hdl_cntread(tk, &st_cmd, &input, vars)) == 2 || ret == 130)
			return (ret);
	ft_strdel(&input);
	if (lexer_ret >= lex_fail)
	{
		free_token_list(tk);
		adjust_history(st_cmd, 1);
		return (lexer_ret);
	}
	ret = handle_execution(st_cmd, tk, vars);
	return (ret == 254 ? 1 : ret);
}
