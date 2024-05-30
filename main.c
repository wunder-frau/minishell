#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
# include "libft_/libft.h"

typedef struct s_node
{
	int				type;
	struct s_node	*left;
	struct s_node	*right;
}	t_node;
typedef int t_bool;

typedef struct s_minishell
{
	char **env;
	char *pwd;
	char *oldpwd;
	t_list *history;
	char *history_path;
	int exit_status;
	t_node *root;
	t_bool is_parent;
	t_bool is_oldpwd_unset;
} t_minishell;

void ft_readline(char **cmdline, char *prompt)
{
	*cmdline = readline(prompt);
}

void init_minishell(t_minishell *shell, char **envp)
{
	shell->env = envp;
	shell->oldpwd = NULL;
	shell->history = NULL;
	shell->history_path = NULL;
	shell->exit_status = 0;
	shell->root = NULL;
	shell->is_parent = 1;
	shell->is_oldpwd_unset = 1;

	char *cmdline = NULL;
	// !! input check
	while (1)
	{
		char *cmdline = NULL;
		ft_readline(&cmdline, "minishell> ");
		if (cmdline)
		{
			if (strcmp(cmdline, "exit") == 0)
			{
				free(cmdline);
				break;
			}
			add_history(cmdline);
			printf("You entered: %s\n", cmdline);
			free(cmdline);
		}
    }
    //.....
    if (cmdline)
	{
		add_history(cmdline);
		free(cmdline);
    }
}

int main(int argc, char **argv, char **envp)
{
	t_minishell shell;
	init_minishell(&shell, envp);
	return 0;
}
