#ifndef MINISHELL_H
# define MINISHELL_H

#include "minishell.h"
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

void	ft_readline(char **cmdline, char *prompt);
void	run_commandline(t_minishell **ms);

#endif