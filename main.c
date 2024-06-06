#include "libft_/libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <errno.h>

typedef struct s_node
{
	int type;
	struct s_node *left;
	struct s_node *right;
} t_node;

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

void	ft_readline(char **cmdline, char *prompt)
{
	*cmdline = readline(prompt);
}

int	check_if_executable(char *cmd)
{
	int	res;

	res = access(cmd, X_OK | F_OK);
	if (res == 0)
		return (1);
	return (0);
}

char	*find_command(char *cmd, char **envp)
{
	char	**paths;
	char	*path_env;
	char	*full_path = NULL;
	int		i;
	int		len;

	if (!envp)
		return (NULL);
	if (check_if_executable(cmd))
		return (ft_strdup(cmd));
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		len = ft_strlen(paths[i]) + ft_strlen(cmd) + 2;
		full_path = (char *)malloc(sizeof(char) * len);
		if (!full_path)
		{
			while (paths[i])
				free(paths[i++]);
			free(paths);
			return (NULL);
		}
		snprintf(full_path, len, "%s/%s", paths[i], cmd);
		if (check_if_executable(full_path))
		{
			while (paths[i])
				free(paths[i++]);
			free(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
	return (NULL);
}

void	init_minishell(t_minishell *shell, char **envp)
{
	int		i;
	char	**argv;
	int		status;
	char	*cmd_path;

	shell->env = envp;
	shell->pwd = getcwd(NULL, 0);
	shell->oldpwd = NULL;
	shell->history = NULL;
	shell->history_path = NULL;
	shell->exit_status = 0;
	shell->root = NULL;
	shell->is_parent = 1;
	shell->is_oldpwd_unset = 1;
	char *cmdline = NULL;
	while (1)
	{
		ft_readline(&cmdline, "minishell> ");
		if (cmdline)
		{
			if (strcmp(cmdline, "exit") == 0)
			{
				free(cmdline);
				break ;
			}
			else
			{
				argv = ft_split(cmdline, ' ');
				if (!argv)
				{
					free(cmdline);
					continue ;
				}
				cmd_path = find_command(argv[0], envp);
				if (cmd_path)
				{
					pid_t pid = fork();
					if (pid == 0)
					{
						// Child process
						if (execve(cmd_path, argv, shell->env) == -1)
						{
							perror("execve");
							exit(EXIT_FAILURE);
						}
					}
					else if (pid > 0)
					{
						// Parent process
						waitpid(pid, &status, 0);
					}
					else
					{
						// Fork failed
						perror("fork");
					}
					free(cmd_path);
				}
				else
				{
					printf("Command not found: %s\n", argv[0]);
				}
				i = 0;
				while (argv[i])
					free(argv[i++]);
				free(argv);
				add_history(cmdline);
				free(cmdline);
			}
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	shell;

	if (argc < 1 || !argv)
		return (0);
	init_minishell(&shell, envp);
	return (0);
}
