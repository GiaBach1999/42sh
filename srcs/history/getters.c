#include "ftsh.h"
#include "history.h"

t_hist_lst		*get_begin_lst(t_hist_lst *hist_lst)
{
	if (!hist_lst)
		return (NULL);
	else
		while (hist_lst->prev)
			hist_lst = hist_lst->prev;
	return (hist_lst);
}

t_hist_lst		*get_end_lst(t_hist_lst *hist_lst)
{
	if (!hist_lst)
		return (NULL);
	else
		while (hist_lst->next)
			hist_lst = hist_lst->next;
	return (hist_lst);
}

int				get_hist_len(t_hist_lst *hist_lst)
{
	int			i;

	if (!hist_lst)
		return (0);
	else
	{
		i = 0;
		hist_lst = get_begin_lst(hist_lst);
		while (hist_lst->next)
		{
			i++;
			hist_lst = hist_lst->next;
		}
		return (i - 1);
	}
}
