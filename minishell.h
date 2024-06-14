#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "libft_/libft.h"

typedef struct s_node
{
	int				type;
	struct s_node	*left;
	struct s_node	*right;
}	t_node;

typedef struct s_node_info
{
	int		type;
	char	*str_left;
	char	*str_right;
}	t_node_info;

typedef struct s_minishell
{
	char **env;
	char *pwd;
	char *oldpwd;
	t_list *history;
	char *history_path;
	int exit_status;
	t_node *root;
	bool is_parent;
	bool is_oldpwd_unset;
} t_minishell;

// ! remove extra
enum	e_characters
{
	NULL_TERM,
	HT = 9,
	NL = 10,
	L_REDIR_SEPARATOR = 29,
	R_REDIR_SEPARATOR = 30,
	SEPARATOR = 31,
	SPACE = 32,
	D_QUOTE = 34,
	D_SIGN = 36,
	AND = 38,
	S_QUOTE = 39,
	O_ROUND = 40,
	C_ROUND = 41,
	ASTERISK = 42,
	DASH = 45,
	DOT = 46,
	SLASH = 47,
	COLON = 58,
	SEMICOLON = 59,
	REDIR_L = 60,
	EQUAL = 61,
	REDIR_R = 62,
	Q_MARK = 63,
	BACKSLASH = 92,
	UNSCORE = 95,
	N_LOWER = 110,
	PIPE = 124,
};

enum	e_types
{
	T_PIPE,
	T_COMMAND,
};

/** run_commanline.c **/
void	ft_readline(char **cmdline, char *prompt);
void	run_commandline(t_minishell **ms);

/** parse_command_tree.c **/
bool	get_type(char *str, t_node_info **info);
int		create_tree(char *str, t_node **root, int *hd_num, t_minishell *ms);

/** lexer.c **/
int		lexer(t_node_info **node, char *str, int type, int i);
int		pipe_block(t_node_info **node, char *str, int type, int i);
int		set_node_info(t_node_info **info, char *str, int point, int type);
int		round_brackets_check(char *str, int point);
void	check_if_inside_quotes(char *str, int *i, int *quote_type);


#endif