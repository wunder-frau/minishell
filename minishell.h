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
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "libft_/libft.h"

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

typedef struct s_parsed_data
{
	int		type;
	char	*block_left;
	char	*block_right;
}	t_parsed_data;

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

typedef struct s_hmap
{
    char            *key;
    char            *value;
    struct s_hmap   *next;
} t_hmap;

typedef struct s_cmd_data
{
	char *cmd;
	char **argv;
	char **cmd_paths;
	char	*cmd_path;
	char *cmd_line;
	t_hmap **hashmap;
} t_cmd_data;

typedef struct s_minishell
{
	char **command;
	t_hmap **hashmap;
//	char **env;
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

enum	e_builtins
{
	NOT_BUILTIN,
	C_EMPTY,
	C_BLANK,
	C_ECHO,
	C_CD,
	C_PWD,
	C_EXPORT,
	C_UNSET,
	C_ENV,
	C_EXIT,
};

enum	e_builtins_len
{
	CD_LEN = 3,
	ENV_LEN = 4,
	PWD_LEN = 4,
	ECHO_LEN = 5,
	EXIT_LEN = 5,
	UNSET_LEN = 6,
	EXPORT_LEN = 7,
};

/** TO DELETE **/
void print_node(t_node *node);

void			rl_replace_line(const char *text, int clear_undo);
/** run_commanline.c **/
void	ft_readline(char **cmdline, char *prompt);
void	run_commandline(t_minishell **ms);
char	**dup_envp(char **envp);
void	terminate_minishell(t_minishell **ms, int status);
void	init_minishell(t_minishell **ms, char **envp, t_cmd_data *cmd_data);

/** parse_ast.c **/
bool	parse_ast(char *str, t_parsed_data **info);
int		build_ast(char *str, t_node **root, int *hd_num, t_minishell *ms);
int		assemble_ast_pipe(t_parsed_data *info, t_node **root, int *hd_num, t_minishell *ms);
int	prepare_redirects(char *redirects_line, int *hd_num, char ***redirs, t_minishell *ms);
int		assemble_ast_command(t_parsed_data *info, t_node **root, int *hd_num, t_minishell *ms);
t_redir *init_node_redir(void);
t_pipe  *init_node_pipe(void);
t_command *init_node_cmd(void);

/** lexer.c **/
int		lexer(t_parsed_data **node, char *str, int type, int i);
int		block_pipe(t_parsed_data **node, char *str, int type, int i);
int		set_node_data_pipe(t_parsed_data **info, char *str, int point, int type);
int		set_node_cmd_redirects(t_parsed_data **info, char *str, int type);
int		set_node_cmd(t_parsed_data **info, char *str, int type);
int		round_brackets_check(char *str, int point);
void	quote_tracker(char *str, int *i, int *is_str);
int		check_symbol_pairing(char *str, int point, int symbol);
bool	init_node(t_parsed_data *info, t_node **root);
bool		check_redirection(char *str);

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

/** utils.c **/
void	remove_quotes(char *str, int i, int j);
void	remove_quotes_arr(char **arr, int i);
bool	is_blank_string(char *str);
char	**split_handle_quotes_and_spaces(char *str);
int		parse_cmd(char *cmd, char ***res, t_minishell *ms);

/** redirects_apply.c **/
int	apply_append(char *redir, t_minishell *ms, int *out);
int	apply_heredoc(char *heredoc, int *in);
int	apply_redir_in(char *redir, t_minishell *ms, int *in);
int	apply_redir_out(char *redir, t_minishell *ms, int *out);
int	apply_redirect(char *redir, t_minishell *ms, int *in, int *out);
int	replace_fd(int in, int out);
int	apply_redirects(char **redirs, t_minishell *ms);

/** traverse_command.c **/
int	traverse_command(char *cmd, char **redir, t_minishell *ms);
int	run_cmd_with_redir(char **command, char **redir, t_minishell *ms);
int	traverse_tree(t_node **root, t_minishell *ms);

/** shlvl.c **/
int	add_to_env_list_new_env(char **envp, char ***result, int *i, int *len);
void	shlvl_warning(int number);
int	env_var(char **envp, char *var, int i, int j);
int	check_for_non_digits(char *str);
int	shlvl_init_when_shlvl_exists(int position, char ***envp, int number, char *shlvl);
int	var_init_when_no_var_exists(char ***envp, int i, char *var);
int	shlvl_init(char ***envp);

/** utils_free.c **/
void	free_arr_2d(void *ptr);
void	free_minishell(t_minishell *ms);
void	free_ast(t_node **root);


int	exec_builtin(t_minishell *shell);
int	find_executable(char **command, char **paths);
int	locate_command(char	**command, char	**envp);
int	is_builtin(char *cmd);
int	run_builtin(char **redir, t_minishell *ms);
int	run_builtin_without_redir(char **command, t_minishell *ms, int cmd_type);
int	exec_builtin_redir(char **command, char **redir, t_minishell *ms);
int	copy_std_fd(int *in_fd, int *out_fd, char *command);
void	return_std_fd(int *in_fd, int *out_fd, int *status, char *command);
void	run_pwd(char **arr, t_minishell *ms);
void	print_arg_err_msg(char *cmd, char *arg, char *msg);


t_hmap **init_hmap(char **env);
void add_new_var(t_hmap **v, char *akey, char *avalue);
void free_hashmap(t_hmap *hashmap);
int key_exists(t_hmap *v, char *ekey);
char *return_value_hash(t_hmap *v, char *key);
char *ft_get_env(t_hmap *hashmap, const char *key);
void ft_pwd(t_minishell *shell);

#endif
