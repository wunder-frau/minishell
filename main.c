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

void ft_readline(char **cmdline, char *prompt)
{
	*cmdline = readline(prompt);
}

int check_if_executable(char *cmd)
{
	int res = access(cmd, X_OK | F_OK);
	if (res == 0)
		return 1;
	return 0;
}

char *find_command(char *cmd, char **envp)
{
	
	if (!envp)
		printf("AZAZA");
	if (check_if_executable(cmd))
		return cmd;
	char *path_env = getenv("PATH");
	if (!path_env)
		return NULL;

	char *path = strdup(path_env);
	char *token = strtok(path, ":");
	while (token)
	{
		char full_path[1024];
		snprintf(full_path, sizeof(full_path), "%s/%s", token, cmd);
		if (check_if_executable(full_path))
		{
			free(path);
			return strdup(full_path);
		}
		token = strtok(NULL, ":");
	}
	free(path);
	return NULL;
}

void init_minishell(t_minishell *shell, char **envp)
{
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
				break;
			}
			else
			{
				char *argv[1024];
				int argc = 0;
				char *token = strtok(cmdline, " ");
				while (token != NULL)
				{
					argv[argc++] = token;
					token = strtok(NULL, " ");
				}
				argv[argc] = NULL;

				char *cmd_path = find_command(argv[0], envp);
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
						int status;
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
				add_history(cmdline);
				free(cmdline);
			}
		}
	}
}

int main(int argc, char **argv, char **envp)
{
	t_minishell shell;

	if (argc < 1 || !argv)
		return 0;

	init_minishell(&shell, envp);
	return 0;
}