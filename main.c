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

// env.c

size_t	ft_arrlen(void **arr)
{
	size_t	length;

	length = 0;
	while (arr[length] != NULL)
		length++;
	return (length);
}

void	ft_free(void *ptr)
{
	void	**arr;
	size_t	i;

	arr = (void **)ptr;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

char	**env_c(char **envp)
{
	char	**env;
	int		len;
	int		i = 0;

	len = ft_arrlen((void **)envp);
	env = calloc(len + 1, sizeof(char *));
	if (!env)
		return (NULL);
	while (i < len)
	{
		env[i] = strdup(envp[i]);
		if (!env[i])
		{
			ft_free((void *)env);
			return (NULL);
		}
		i++;
	}
	i = 0;

	while(i < len && env[i] != NULL)
	{
		printf("%s\n", env[i]);
		i++;
	}
	return (env);
}

void ft_readline(char **cmdline, char *prompt)
{
	*cmdline = readline(prompt);
}

void init_minishell(t_minishell *shell)
{
	extern char	**environ;
	int			status;

	status = 0;
	shell = calloc(1, sizeof(t_minishell));
	// if(!shell)
	// 	return (0);
	shell->env = env_c(environ);
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
	init_minishell(&shell);
	return 0;
}
