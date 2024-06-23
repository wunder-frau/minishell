#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef HEREDOC_NAME
#  define HEREDOC_NAME "<<.heredoc_"
# endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "libft_/libft.h"

enum	e_signals
{
	DEFAULT,
	INTERACTIVE,
	HEREDOC,
	IGNORE,
};

enum	e_signals_echo
{
	IMPLICIT,
	EXPLICIT,
};

enum	e_exit_status
{
	SUCCESS,
	GENERIC_ERROR,
	CMD_ARG_ERROR,
	CMD_PD_FAILURE = 126,
	CMD_NF_FAILURE = 127,
	MALLOC_ERR = 200,
	SYSTEM_ERROR = 201,
	CHDIR_ERROR = 202,
	GETCWD_ERROR = 203,
	UNEXPECTED_EXIT = 255,
	SYNTAX_ERROR = 258,
	EXECVE_FAILURE = 300,
	FORK_FAILURE = 400,
	PIPE_FAILURE = 500,
	DUP_FAILURE = 600,
};

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

typedef struct s_pipe
{
	int		type;
	t_node	*left;
	t_node	*right;
}	t_pipe;

typedef struct s_command
{
	int		type;
	t_node	*redir;
	char	*cmd;
}	t_command;

typedef struct s_redir
{
	int		type;
	char	**redirs;
}	t_redir;

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

enum	e_characters
{
	NULL_TERM,
	HT = 9,
	NL = 10,
	L_REDIR_SEPARATOR = 29,
	R_REDIR_SEPARATOR = 30,
	SEPARATOR = 31,
	SPA = 32,
	D_QUO = 34,
	D_SIGN = 36,
	AND = 38,
	S_QUO = 39,
	O_ROU = 40,
	C_ROU = 41,
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
	PI = 124,
};

enum	e_types
{
	T_PIPE,
	T_COMMAND,
	T_REDIR,
};

void	terminate_minishell(t_minishell **ms, int status);

/** run_commanline.c **/
int	ft_strcmp(const char *s1, const char *s2);
void	ft_readline(char **cmdline, char *prompt);
int		get_cmdline(char **cmdline, t_minishell **ms);
void	run_commandline(t_minishell **ms);

/** parse_command_tree.c **/
bool	get_type(char *str, t_node_info **info);
int		create_tree(char *str, t_node **root, int *hd_num, t_minishell *ms);
int		pipe_tree(t_node_info *info, t_node **root, int *hd_num, t_minishell *ms);
int	prepare_redirects(char *redirects_line, int *hd_num, char ***redirs, t_minishell *ms);
int		add_command(t_node_info *info, t_node **root, int *hd_num, t_minishell *ms);
void	free_tree(t_node **root);
void	ft_free_2d_array(void *ptr);
t_redir *init_t_redir(void);
t_pipe  *create_pipe_node(void);
t_command *init_t_command(void);

/** lexer.c **/
int		ft_isspace(char c);
int		lexer(t_node_info **node, char *str, int type, int i);
int		pipe_block(t_node_info **node, char *str, int type, int i);
int		set_node_info(t_node_info **info, char *str, int point, int type);
int		set_node_info_command(t_node_info **info, char *str, int type);
int		set_node_cmd_simple(t_node_info **info, char *str, int type);
int		round_brackets_check(char *str, int point);
void	check_if_inside_quotes(char *str, int *i, int *quote_type);
int		check_symbol_pairing(char *str, int point, int symbol);
bool	create_node(t_node_info *info, t_node **root);
int		redir_search(char *str);

/** lexer_utils.c **/
int	modificate_str_command_without_br(char *str, char **redir, int i, int j);

/** prepare_redirects.c **/
int		check_redir(char **redir, t_minishell *ms);
void	print_err_msg(char *cmd, char *msg);
int		prepare_redirects(char *redirects_line, int *hd_num, char ***redirs, t_minishell *ms);
void	perror_err_msg(char *cmd, char *arg);
char	*get_hd_name(int *hd_num);
void	remove_hd_files(int *hd_num);

/** prepare_heredocs.c **/
int		prepare_heredocs(char ***redirs, int *hd_num, t_minishell *ms);
void	remove_hd_duplicates(char ***redirs, char *hd_name, char hd_counter);
int		prepare_heredoc(char **limiter, char *hd_name, t_minishell *ms);

/** wait_children.c **/
int		wait_children(pid_t *pids, int num);
void	print_msg(int status);

/** utils.c **/
void	remove_quotes(char *str, int i, int j);
void	remove_quotes_arr(char **arr, int i);
bool	is_blank_string(char *str);
char	**wrapper_ft_split_with_quotes(char *str);
int		parse_cmd(char *cmd, char ***res, t_minishell *ms);
size_t	ft_arrlen(void **arr);

/** redirects_apply.c **/
int	apply_append(char *redir, t_minishell *ms, int *out);
int	apply_heredoc(char *heredoc, int *in);
int	apply_redir_in(char *redir, t_minishell *ms, int *in);
int	apply_redir_out(char *redir, t_minishell *ms, int *out);
int	apply_redirect(char *redir, t_minishell *ms, int *in, int *out);
int	replace_fd(int in, int out);
int	apply_redirects(char **redirs, t_minishell *ms);

#endif
