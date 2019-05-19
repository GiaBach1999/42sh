#include "lexer.h"
#include "ast.h"

/*t_bool	is_quote_token(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == TK_DQ_STR || token->type == TK_SQ_STR)
		return (1);
	return (0);
}*/

/*
**	Returns 0 if token is a pipe (or bigger), or an eat token.
**	Returns 1 on quotes, words, monochars, and redirections.
*/

t_bool		is_argv_token(t_token *probe)
{
	if (!probe)
		return (0);
	if (probe->type > tk_eat && probe->type < tk_pipe)
		return (1);
	return (0);
}

/*
**	Returns 0 if token is bigger than a pipe.
**	Else returns 1.
*/

t_bool	is_simple_cmd_token(t_token *probe)
{
	if (!probe)
		return (0);
	if (probe->type < tk_pipe)
		return (1);
	else
		return (0);
}
