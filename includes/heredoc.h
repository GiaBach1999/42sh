#ifndef HEREDOC_H
# define HEREDOC_H

# include "ftsh.h"
# include "lexer.h"

# define ERR_PARSE_EOF 2
# define CTRL_D_ERR ": warning: here-document delimited by end-of-file (wanted"

char	*eof_join(char *eof, t_token *token);
int		parse_heredoc(t_token *token_head, t_vars *vars);
char	*get_doc(char *eof, int is_eof_quoted, t_vars *vars);
char	*write_heredoc_in_file(char **txt);
char	*concatenate_heredoc_txt(t_st_cmd *st_cmd, t_st_cmd *start);
char	*free_get_doc(t_st_cmd *cmds[2], char *txt, char *eof);
void	clean_heredoc(t_st_cmd *st_cmd, t_st_cmd *heredoc_start);
int		is_e(int len, char *txt, char *eof);
int		get_doc_ctrl_d(int *ctrl_d, char *eof, char **txt, t_st_cmd *cmds[2]);
#endif
