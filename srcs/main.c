#include "builtins.h"
#include "line_editing.h"
#include "hashmap.h"
#include "history.h"
#include "signals.h"
#include "jobs.h"

int				g_isatty;
struct termios	g_saved_attr;
struct termios	g_42sh_attr;

int			is_full_of_whitespaces(const char *input)
{
	int		i;

	if (!input)
		return (1);
	i = -1;
	while (input[++i])
	{
		if (!ft_is_white_space(input[i]))
			return (0);
	}
	return (1);
}

/*
**	Utility function to free all alloacted variables and reset the old
**	terminal attributes.
**	Returns the last cmd value.
*/

static int	free_variables(t_vars *vars, t_st_cmd *st_cmd)
{
	int	ret;

	print_exit();
	write_to_history(st_cmd, (const char **)vars->env_vars);
	free_all_st_cmds(&st_cmd);
	ret = vars->cmd_value;
	free_vars(vars);
	reset_terminal_settings();
	return (ret);
}


void	print_jobs(t_job *j)
{
	t_token *probe;
	ft_dprintf(2, "-- JOBS --\n");
	while (j)
	{
		probe = j->token_list;
		while (probe)
		{
			ft_dprintf(2, "%s\n", probe->content);
			probe = probe->next;
		}
		ft_dprintf(2, "%p\n", j->first_process);
		j = j->next;
	}
}

/*
**	Initialize variables, read input with input_loop.
**	If reading input fails or first byte of input is 0, exits.
**	Else, calls handle_input for lexing, creating and executing the AST.
**	On exit, writes history and frees everything that was previously allocated.
*/

int			main(int argc, char **argv, char **env)
{
	t_st_cmd		*st_cmd;
	t_vars			vars;
	int				ret;

	init_shell();
	if (setup_terminal_settings() > 0)
		return (EXIT_FAILURE);
	if (init_vars(&vars, argc, argv, env) == 1)
		return (EXIT_FAILURE);
	print_introduction();
	st_cmd = init_st_cmd((const char **)vars.env_vars);
	get_st_cmd(&st_cmd);
	while (42)
	{
		if ((ret = input_loop(st_cmd, &vars, regular)) == 0
				|| !*st_cmd->st_txt->txt)
			break ;
		else if (ret > 0 && !is_full_of_whitespaces(st_cmd->st_txt->txt))
			vars.cmd_value = handle_input(st_cmd, &vars);
		do_job_notification();
		st_cmd = reset_st_cmd(st_cmd);
	}
	ret = free_variables(&vars, st_cmd);
	return (ret);
}
