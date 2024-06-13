#include "minishell.h"
#include "libft_/libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <errno.h>

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

void	init_minishell(t_minishell *shell, char **envp)
{
	int		i;
	char	**argv;
	int		status;
	char	*cmd_path = NULL;
	char 	*cmdline = NULL;

	shell->env = envp;
	shell->pwd = getcwd(NULL, 0);
	shell->oldpwd = NULL;
	shell->history = NULL;
	shell->history_path = NULL;
	shell->exit_status = 0;
	shell->root = NULL;
	shell->is_parent = 1;
	shell->is_oldpwd_unset = 1;
	shell->fd_output = 1;
	while (1)
	{
		ft_readline(&cmdline, "minishell> ");
		if (cmdline)
		{
			if (ft_strncmp(cmdline, "exit", 4) == 0)
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
			printf("argv[0]: %s\n", argv[0]);
			pid_t pid = fork();
			if (pid == 0)
			{
				// Child process
				if (is_builtin(argv[0]) == 1)
					exec_builtin(shell, argv);
				/*else if (execve(cmd_path, argv, shell->env) == -1)
				{
					perror("execve");
					exit(EXIT_FAILURE);
				}*/
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
