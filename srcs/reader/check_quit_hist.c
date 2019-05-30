#include "input.h"
#include "signals.h"
#include "history.h"
#include "line_editing.h"
#include "builtins.h"
#include "libft.h"

/*
**	Returns 1 if ctrld is pressed and line is empty
**	Returns 0 if the key is not ctrld
**	Returns -1 if ctrld is pressed but line is not empty
*/

int		check_for_quit(t_st_cmd *st_cmd, const char *buf)
{
	if (ft_strncmp(buf, CTRL_D, 2) == 0)
	{
		if (st_cmd->st_txt->data_size == 0)
		{
			if (st_cmd->prev)
				ft_dprintf(STDERR_FILENO, "42sh: unexpected EOF\n");
			*st_cmd->st_txt->txt = 0;
			return (1);
		}
		else if (st_cmd->st_txt->tracker >= st_cmd->st_txt->data_size)
			return (-1);
		else
			delete_right(st_cmd);
		return (-1);
	}
	else
		return (0);
}

int					strstr_adapted(const char *haystack, const char *needle)
{
	size_t			i;
	size_t			j;

	if (*needle == 0)
		return (0);//pas possible d'utiliser ft_strstr car dans ce cas, il renvoit haystack
	i = 0;
	while (haystack[i])
	{
		j = 0;
		while (haystack[i] == needle[j] && needle[j])
		{
			i++;
			j++;
		}
		if (needle[j] == 0)
			return (1);
		i = i - j + 1;
	}
	return (0);
}

/*
**	Looking for pattern "to_find" in st_cmd->hist_lst, recursively
**	Returns 0 if to_find is not found
**	Returns 1 if to_find is found
*/


int					search_reverse_in_histo(t_st_cmd **st_cmd, char *to_find)
{
	if ((*st_cmd)->hist_lst->next == NULL)//check si on est sur last ou non pour savoir a partir d'ou chercher. il faut commencer la ou on est actuellement.
		(*st_cmd)->hist_lst = (*st_cmd)->hist_lst->prev;// il faut garder en memoire la place ou on est : reculer st_cmd->hist_lst ? OUI (si on remonte de 3 maillons avec ctrl R et qu'on appuie sur fleche du haut, alors on est a head - 4
	while ((*st_cmd)->hist_lst)
	{
		if (strstr_adapted((*st_cmd)->hist_lst->txt, to_find) == 1)//pattern found -> je dois placer le curseur sur la derniere occurence du pattern et return 1 pour afficher le prompt SUCCESS
		{
			// free txt
			if (!((*st_cmd)->st_txt->txt = ft_strdup((*st_cmd)->hist_lst->txt)))//change st_cmd->st_txt->txt par l'entree de l'historique correspondant
				ERROR_MEM
			(*st_cmd)->st_txt->data_size = ft_strlen((*st_cmd)->st_txt->txt);//change st_cmd->data_size par strlen
			(*st_cmd)->st_txt->tracker = ft_strlen((*st_cmd)->st_txt->txt) - ft_strlen(ft_strrstr((*st_cmd)->st_txt->txt, to_find));//change st_cmd->tracker par ft_strlen(txt) - ft_strlen(ft_strrstr)
			return (0);
		}
		if ((*st_cmd)->hist_lst->prev)
			(*st_cmd)->hist_lst = (*st_cmd)->hist_lst->prev;
		else
			break ;
	}
	return (1);

}

int			ft_isprint_ctrlr(int c)
{
	if ((c > 31 && c < 127) || c == 8 || c == ' ' )
		return (1);
	return (0);
}

/*
**	If buf_received == ctrlr, reverse-i-search in historic
**	Returns 0 if the key is not ctrlr 
**	Returns 1 if it is ctrlr + display new prompt + search
*/

int		check_for_search_histo(t_st_cmd *st_cmd, const char *buf_received)
{
	char			*buf;
	char			c;
	int				ret;
	size_t			i;
	t_st_cmd		*to_find;
	size_t			malloc_size;
	int				prompt_type;

	to_find = NULL;
	i = 0;
	malloc_size = 256;//voir avec scott pour use ft_realloc
	prompt_type = 0;
	if (ft_strncmp(buf_received, CTRL_R , 2) == 0)
	{
		if (!(buf = ft_strnew(malloc_size + 1)))
			ERROR_MEM
		ft_bzero(buf, malloc_size + 1);
		print_prompt_search_histo(st_cmd, buf, prompt_type);
		while ((ret = read(STDIN_FILENO, &c, 1)) > 0)
		{
			if (!ft_isprint_ctrlr(c))// quitte la boucle en cas de fleche etc... -> pas de termcaps. Il faut peut etre le join a st_cmd->st_txt->txt quand meme (ex : si on met "shift + fleche de gauche", le ctrl R doit quitter et le prompt doit afficher D ; EN FAIT pas que ascii (exemple TAB quitte)
			{
				// 3 cas : ctrlc -> affiche ctrlc, nouveau prompt une ligne en dessous, entree -> substitue par un promp regular le prompt de recherche et execute, char de merdre -> overwrite un prompt sur la meme ligne
				if (c == '\n')//ENTER
				{
				free_st_prompt(&st_cmd->st_prompt);
				st_cmd->st_prompt = init_st_prompt(NULL);	
				execute_str(BEGIN_LINE);
				execute_str(CLEAR_BELOW);
				print_prompt(st_cmd);
				st_cmd->st_txt->tracker = 0;
				write_st_cmd(st_cmd);
				return (1);
				}



				else if (c ==  3)
				{
				free_st_prompt(&st_cmd->st_prompt);
				st_cmd->st_prompt = init_st_prompt(NULL);	
				execute_str(BEGIN_LINE);
				execute_str(CLEAR_BELOW);
				print_prompt(st_cmd);
				st_cmd->st_txt->tracker = 0;
				write_st_cmd(st_cmd);

				}
				else
				{
				free_st_prompt(&st_cmd->st_prompt);
				st_cmd->st_prompt = init_st_prompt(NULL);	
				execute_str(BEGIN_LINE);
				execute_str(CLEAR_BELOW);
				print_prompt(st_cmd);
				st_cmd->st_txt->tracker = 0;
				write_st_cmd(st_cmd);

				}
				break ;
			}
			buf = ft_realloc(buf, ft_strlen(buf) - 1, &malloc_size, 1);
			buf[ft_strlen(buf)] = c;//buf contient ce que je dois chercher dans l'historique ; gere quand c'est del, etc..
			execute_str(BEGIN_LINE);
			execute_str(CLEAR_BELOW);
			prompt_type = search_reverse_in_histo(&st_cmd, buf);//pattern found ; afficher le prompt SUCCESS (reverse-i-search)`$buf': $st_cmd->st_txt->txt
			print_prompt_search_histo(st_cmd, buf, prompt_type);
		/*	retrieve_pos(&st_cmd->start_pos);
			write_st_cmd(st_cmd);
			get_pos(st_cmd, st_cmd->st_txt->tracker);
		*/	

		}
		return (1);
	}
	else
	{
		return (0);
	}
}
