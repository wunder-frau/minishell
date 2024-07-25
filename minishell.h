/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 11:24:28 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/25 09:37:50 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef HEREDOC_NAME
#  define HEREDOC_NAME "<<.heredoc_"
# endif

# include <termios.h>
# include <stdlib.h>
# include <errno.h>
# include <sys/wait.h>
# include <stdio.h>
# include <stdbool.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "libft_/libft.h"

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
	char					*key;
	char					*value;
	struct s_hmap			*next;
}	t_hmap;

typedef struct s_cmd_data
{
	char		*cmd;
	char		**argv;
	char		**cmd_paths;
	char		*cmd_path;
	char		*cmd_line;
	t_hmap		**hashmap;
}	t_cmd_data;

typedef struct s_minishell
{
	int					fd_output;
	char				**command;
	t_hmap				**hashmap;
	char				*pwd;
	char				*oldpwd;
	int					exit_status;
	t_node				*root;
	bool				is_parent;
	bool				is_oldpwd_unset;
	t_cmd_data			cmd_data;
}	t_minishell;

typedef struct s_expansion_state
{
	char			**result;
	char			**str;
	int				*i;
	int				last_status;
	t_minishell		*ms;
	int				*in_single_quotes;
	int				*in_double_quotes;
}	t_expansion_state;

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

/** readline **/
void			rl_replace_line(const char *text, int clear_undo);

/** ast **/
int				assemble_ast_pipe(t_parsed_data *data, t_node **root, int *hd_c,
					t_minishell *shell);
int				assemble_ast_command(t_parsed_data *data, t_node **root,
					int *hd_c, t_minishell *shell);
t_pipe			*init_node_pipe(void);
t_command		*init_node_cmd(void);
t_redir			*init_node_redir(void);
bool			init_node(t_parsed_data *data, t_node **root);
bool			parse_ast(char *str, t_parsed_data **data);
int				build_ast(char *str, t_node **root, int *hd_c,
					t_minishell *shell);

/** builtins **/
int				run_builtin(char **redir, t_minishell *shell);
int				exec_builtin_redir(char **command, char **redir,
					t_minishell *shell);
int				copy_std_fd(int *in_fd, int *out_fd, char *command);
void			return_std_fd(int *in_fd, int *out_fd, int *status,
					char *command);
bool			is_non_digit_space_sign(const char *str);
bool			is_non_empty_after_trim(const char *str);
void			exit_numeric_arg_error(t_minishell *shell);
void			exit_amount_of_arg_error(t_minishell *shell);
void			clean_and_exit(t_minishell *shell);
void			handle_multiple_args(char **arg, t_minishell *shell);
void			handle_single_arg(char *arg, t_minishell *shell);
void			ft_exit(char **arg, t_minishell *shell);
void			ft_pwd(t_minishell *shell);
void			ft_echo(char **cmd);
void			ft_export(t_minishell *shell, char **argv);
void			ft_unset(t_minishell *shell, char **argv);
void			ft_env(t_minishell *shell);
void			ft_cd(t_minishell *shell, char **argv);

/** helpers **/
void			print_err_msg(char *cmd, char *msg);
void			perror_err_msg(char *cmd, char *arg);
void			print_arg_err_msg(char *cmd, char *arg, char *msg);
void			free_arr_2d(void *ptr);
void			free_minishell(t_minishell *shell);
void			free_ast(t_node **root);
bool			is_inside_quotes(char c, int *inside_quotes);
void			remove_quotes(char *str, int i, int j);
void			remove_quotes_arr(char **arr, int i);
char			**split_handle_quotes_and_spaces(char *str);
void			free_hmap(t_hmap	*node);

/** lexer **/
bool			check_redirection(char *str);
int				set_node_data_pipe(t_parsed_data **data, char *str, int point,
					int type);
int				set_node_cmd_redirects(t_parsed_data **data, char *str,
					int type);
int				set_node_cmd(t_parsed_data **data, char *str, int type);
void			quote_tracker(char *str, int *i, int *quote_status);
int				convert_input_to_redirects(char *str, char **redir, int i,
					int j);
int				check_symbol_pairing(char *str, int point, int symbol);
int				block_pipe(t_parsed_data **node, char *str, int type, int i);
int				block_cmd(t_parsed_data **node, char *str, int type);
int				lexer(t_parsed_data **node, char *str, int type, int i);

/** redirects **/
void			remove_spaces_and_quotes_hd(char *hd);
int				prepare_heredoc(char **limiter, char *hd_name,
					t_minishell *shell);
int				apply_heredoc(char *heredoc, int *in);
int				apply_append(char *redir, t_minishell *shell, int *out);
int				apply_redir_in(char *redir, t_minishell *shell, int *in);
int				apply_redir_out(char *redir, t_minishell *shell, int *out);
int				apply_redirect(char *redir, t_minishell *shell, int *in,
					int *out);
int				apply_redirects(char **redirs, t_minishell *shell);
void			remove_hd_duplicates(char ***redirs, char *hd_name, char hd_c);
int				prepare_heredocs(char ***redirs, int *hd_c, t_minishell *shell);
char			*get_hd_name(int *hd_c);
void			remove_hd_files(int *hd_c);
int				parse_cmd(char *cmd, char ***res, t_minishell *shell);
int				check_redir(char **redir, t_minishell *shell);
int				prepare_redirects(char *redirects_line, int *hd_c,
					char ***redirs, t_minishell *shell);
int				replace_fd(int in, int out);

/** run **/
void			init_minishell(t_minishell **shell, char **envp,
					t_cmd_data *cmd_data);
void			terminate_minishell(t_minishell **shell, int status);
int				main(int argc, char **argv, char **envp);
void			ft_readline(char **cmdline, char *prompt);
void			run_commandline(t_minishell **shell);
void			command_not_found_error(t_minishell *shell);
bool			is_builtin(char *cmd);
int				exec_builtin(t_minishell *shell);

/** cd.c **/
int				change_directory(t_minishell *shell, char **argv, char *home);
void			handle_chdir_error(t_minishell *shell, char *arg, char *oldpwd);
void			update_environment_variables(t_minishell *shell,
					char *oldpwd, char *pwd);
void			ft_cd(t_minishell *shell, char **argv);

/** cd_2.c **/
void			handle_too_many_arguments(t_minishell *shell, char **argv);
char			*get_current_directory(t_minishell *shell);

/** syntax_checker **/
int				syntax_checker_expression(char *str);
char			*syntax_checker_pipeline(char *str, bool *status);
char			*syntax_checker_command(char *str, bool *status);
char			*syntax_checker_redirect(char *str, bool *status);
char			*syntax_checker_word(char *str, bool *status);
char			*handle_quoted_string(char *str, bool *status);
bool			is_blank_string(char *str);
bool			is_alnum_or_quote(char c);
void			print_error_message(char *str);
bool			is_special_token(char *str, int *length);
void			handle_syntax_error(char *str);

/** traverse_tree **/
int				traverse_command(char *cmd, char **redir, t_minishell *shell);
int				run_external_with_redir(char **command, char **redir,
					t_minishell *shell);
int				traverse_tree(t_node **root, t_minishell *shell);
int				traverse_pipe(t_node **root, t_minishell *shell);
int				traverse_lhs(t_node **node, t_minishell *shell,
					int pipefd[2], int pids[2]);
int				traverse_rhs(t_node **node, t_minishell *shell,
					int pipefd[2], int pids[2]);
int				wait_children_and_fetch_exit_status(pid_t *pids, int num);

/** env.c **/
t_hmap			**init_hmap(char **env);
void			add_new_var(t_hmap **v, char *akey, char *avalue);
void			free_hashmap(t_hmap *hashmap);
int				key_exists(t_hmap *v, char *ekey);
char			*return_value_hash(t_hmap *v, char *key);
char			*ft_get_env(t_hmap *hashmap, const char *key);
void			ft_add_env_hash(t_hmap **hashmap, char *key, char *value);
void			ft_remove_env_hash(t_hmap **hashmap, char *key);

/** signals.c **/
void			set_signals(void);
int				ctrl_d_handler(char *input);

/** shlvl.c **/
void			add_shlvl(t_minishell *shell);

/** dollar_expansion.c **/
int				dollar_expansion(char **str, t_minishell *shell, \
int last_status);

/**dollar_expansion_2.c**/
char			*get_var_name(const char *str, int *var_len);
char			*get_status_string(int last_status);
char			*expand_variable(const char *str, t_minishell *ms, \
	int last_status, int *var_len);
void			append_char_to_result(char **result, char c);

/**dollar_expansion_3.c**/
void			process_single_quote(t_expansion_state *state, char c);
void			process_double_quote(t_expansion_state *state, char c);
void			process_dollar(t_expansion_state *state);
void			process_backslash_dollar(t_expansion_state *state);
void			process_other_chars(t_expansion_state *state);

/**pipe_execve.c **/
void			fill_env_array(char **env_array, \
t_hmap *hashmap);
void			handle_cmd_path(t_minishell *shell, \
char **argv, t_cmd_data *cmd_data);
void			handle_exec_status(t_minishell *shell, t_cmd_data *cmd_data);
void			execution(t_minishell *shell, char **argv, \
t_cmd_data *cmd_data);

/**pipe_execve_2.c **/
int				check_if_executable(char *cmd);
char			*get_cmd_path(char **cmd_paths, char *cmd);
char			**allocate_env_array(int count);
int				count_hashmap_entries(t_hmap *hashmap);
char			**convert_hashmap(t_hmap *hashmap);

#endif
