#include "lexer.h"
#include "cmd_parsing.h"
#include <fcntl.h>

static int	print_fd_error(int fd, int mode)
{
	if (mode != -1)
		ft_dprintf(2, "42sh: %d: bad file descriptor\n", fd);
	return (1);
}

/*
**	Returns 0 on success
**	Else returns error number
*/

int			redir_dgreat(t_token *redir, t_token *prev, int mode)
{
	int		old_fd;
	t_token	*next;
	int		new_fd;

	if ((old_fd = check_fd_prev(prev)) < 0)
		old_fd = STDOUT_FILENO;
	if (old_fd > 9)
		return (print_fd_error(old_fd, mode));
	next = redir->next;
	while (next->type == tk_eat)
		next = next->next;
	if ((new_fd = open(next->content, O_WRONLY | O_CREAT | O_APPEND,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0)
	{
		if (mode != -1)
			ft_dprintf(2, "error opening file : %s\n", next->content);
		return (1);
	}
	redirect(new_fd, old_fd, mode);
	redir->type = tk_eat;
	next->type = tk_eat;
	return (0);
}
