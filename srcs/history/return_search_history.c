#include "input.h"
#include "libft.h"
#include "line_editing.h"
#include "history.h"

static int	interrupt_search(t_st_cmd *st_cmd)
{
	if (!(st_cmd->st_txt->txt[0]))
		st_cmd->st_txt->txt[0] = ' ';
	get_pos(st_cmd, st_cmd->st_txt->tracker);
	reposition_cursor(st_cmd);
	if (isatty(STDIN_FILENO))
		write(STDIN_FILENO, "^C", 2);
	execute_str(PRINT_LINE);
	st_cmd->hist_lst = get_end_lst(st_cmd->hist_lst);
	return (ctrl_c_case);
}

static int	case_return_newline(t_st_cmd *st_cmd)
{
	if (!(st_cmd->st_txt->txt[0]))
		st_cmd->st_txt->txt[0] = ' ';
	st_cmd->st_txt->txt = ft_realloc(st_cmd->st_txt->txt,
			st_cmd->st_txt->data_size, &st_cmd->st_txt->malloc_size, 1);
	st_cmd->st_txt->txt[st_cmd->st_txt->data_size] = '\n';
	execute_str(PRINT_LINE);
	return (enter_case);
}

static int	get_good_case_to_ret(char buf, t_st_cmd *st_cmd)
{
	if (buf == '\r' || buf == '\n')
		return (case_return_newline(st_cmd));
	else
	{
		st_cmd->hist_lst = get_end_lst(st_cmd->hist_lst);
		return (quit_case);
	}
}

int			switch_and_return(char buf, t_st_cmd *st_cmd)
{
	char	*newcmd;
	t_vars	*vars;
	size_t	tmp;

	tmp = st_cmd->st_txt->tracker;
	if (buf == 3)
		return (interrupt_search(st_cmd));
	free_st_prompt(&st_cmd->st_prompt);
	st_cmd->st_prompt = init_st_prompt(NULL);
	if (buf == '\r' || buf == '\n')
		newcmd = ft_strjoin(st_cmd->st_txt->txt, "\n");
	else
		newcmd = ft_strdup(st_cmd->st_txt->txt);
	if (!newcmd)
		ERROR_MEM;
	vars = get_vars(NULL);
	move_cursor(st_cmd->start_pos.col, st_cmd->start_pos.row);
	ft_printf("%s%s%s", vars->cmd_value ? RED : GREEN,
			st_cmd->st_prompt->prompt, FG_DFL);
	switch_st_cmd(st_cmd, newcmd);
	st_cmd->st_txt->tracker = tmp;
	get_pos(st_cmd, st_cmd->st_txt->tracker);
	reposition_cursor(st_cmd);
	free(newcmd);
	return (get_good_case_to_ret(buf, st_cmd));
}

int			handle_quitting_chars_and_bcksp(char buf, char **stock)
{
	if (buf == '\x7f' && (*stock)[0])
		(*stock)[ft_strlen(*stock) - 1] = '\0';
	else if (buf != '\x7f')
	{
		ft_strdel(stock);
		return (1);
	}
	return (0);
}
