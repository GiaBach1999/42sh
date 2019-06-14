#include "input.h"
#include "line_editing.h"

/*
**	Moves every character in the string `shift_no' bytes to the left
*/

void	shift_chars_left(char *str, unsigned int shift_no)
{
	size_t i;

	i = 0;
	while (str[i] && str[i + shift_no])
	{
		str[i] = str[i + shift_no];
		i++;
	}
	str[i] = str[i + shift_no];
	while (shift_no)
	{
		str[i + shift_no] = 0;
		shift_no--;
	}
}

/*
**	Deletes the character located on the left side of the cursor. Equivalent to
**	pressing backspace on a regular shell.
*/

void			delete_left(t_st_cmd *st_cmd)
{
	t_st_txt	*st_txt;
	size_t		tmp;

	st_txt = st_cmd->st_txt;
	st_txt->data_size -= 1;
	st_txt->tracker--;
	tmp = st_txt->tracker;
	reposition_cursor(st_cmd, st_txt->tracker);
	execute_str(CLEAR_BELOW);
	shift_chars_left(&st_txt->txt[st_txt->tracker], 1);
	write_st_cmd(st_cmd);
	st_txt->tracker = tmp;
}

/*
**	Deletes the character located on the right side of the cursor. Equivalent to
**	pressing delete on a regular shell.
*/

void			delete_right(t_st_cmd *st_cmd)
{
	t_st_txt	*st_txt;
	size_t		tmp;

	st_txt = st_cmd->st_txt;
	st_txt->data_size -= 1;
	tmp = st_txt->tracker;
	reposition_cursor(st_cmd, st_txt->tracker);
	execute_str(CLEAR_BELOW);
	shift_chars_left(&st_txt->txt[st_txt->tracker], 1);
	write_st_cmd(st_cmd);
	st_txt->tracker = tmp;
}
