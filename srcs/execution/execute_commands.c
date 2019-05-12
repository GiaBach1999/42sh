#include "42sh.h"
#include "builtins.h"
#include "execution.h"
#include "libterm.h"
#include "cmd_parsing.h"

static t_bool		execute_argv(char **argv, t_vars *vars)
{
	int		cmd;
	char	*cmd_path;

	if (!argv)
		return (0);
	if ((cmd = check_builtins(argv)))
		return (exec_builtins(argv, vars, cmd));
	if (!(cmd_path = check_hashmap(argv[0], vars->hashmap, HASH_EXEC)))
		if (!(cmd_path = get_cmd_path(argv, vars->env_vars)))
			return (0); // error msg ? not found
	if (reset_terminal_settings())
		execve(cmd_path, (char * const*)argv, (char* const*)vars->env_vars);
	clean_exit(1);
	return (1);
}

t_bool		execute_in_fork(t_token *token_head, int in, int out, t_vars *vars)
{
	char		**argv;

	redirect(in, STDIN_FILENO);
	redirect(out, STDOUT_FILENO);
	parse_expands(token_head, vars->env_vars);
	parse_redirections(token_head);
	argv = get_argv_from_token_lst(token_head);
	return (execute_argv(argv, vars));
}

static void	fake_redir_parser(t_token *token_head)
{
	t_token *current;
	t_token *prev;
	t_token	*next;

	current = token_head;
	if (!current)
		return ;
	prev = NULL;
	while (is_simple_cmd_token(current))
	{
		if (current->type == TK_REDIRECTION)
		{
			next = current->next;
			while (next->type == TK_EAT)
				next = next->next;
			current->type = TK_EAT;
			next->type = TK_EAT;
		}
		current = current->next;
	}
}

t_bool		execute_only_one_cmd(t_token *token_head, t_vars *vars)
{
	t_token					*cpy;
	char					**argv;
	unsigned int			cmd;
	unsigned int			ret;
	char					*cmd_path;

	cpy = copy_tokens(token_head);
	parse_expands(cpy, vars->env_vars);
	fake_redir_parser(cpy);
	argv = get_argv_from_token_lst(cpy);
	if ((cmd = check_builtins(argv)))
	{
		ft_free_ntab(argv);
		parse_expands(token_head, vars->env_vars);
		parse_redirections(token_head);
		argv = get_argv_from_token_lst(token_head);
		reset_terminal_settings();
		ret = exec_builtins(argv, vars, cmd);
		ft_free_ntab(argv);
		setup_terminal_settings();
	}
	else
	{
		if (!(cmd_path = get_cmd_path(argv, vars->env_vars)))
		{
			ft_free_ntab(argv);
			free_token_list(cpy);
			return (0);// if cmd_path is NULL  and error msg
		}
		add_to_hashmap(argv[0], cmd_path, &vars->hashmap);
		ret = 1;
		ft_free_ntab(argv);
	}
	free_token_list(cpy);
	return (ret);
}