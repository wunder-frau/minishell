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

static t_hmap	*hmap_new(char *akey, char *avalue)
{
	t_hmap	*node;

	if (!akey)
		return (NULL);
	node = (t_hmap *)malloc(sizeof(t_hmap));
	if (!node)
	{
		ft_putstr_fd("Memory allocation failed.\n", 2);
		exit(EXIT_FAILURE);
	}
	node->key = ft_strdup(akey);
	if (!avalue)
		node->value = ft_strdup("");
	else
		node->value = ft_strdup(avalue);
	node->next = NULL;
	return (node);
}

void	add_new_var(t_hmap **v, char *akey, char *avalue)
{
	t_hmap	*node;
	t_hmap	*temp;

	if (akey[0] == ' ')
		node = hmap_new(akey + 1, NULL);
	else
		node = hmap_new(akey, avalue);
	if (!node)
		return ;
	if (*v == NULL)
		*v = node;
	else
	{
		temp = *v;
		while (temp->next)
			temp = temp->next;
		temp->next = node;
	}
}

t_hmap	**init_hmap(char **env)
{
	char	*hash_key;
	t_hmap	**hashmap;
	int		j;
	int		i;

	hashmap = (t_hmap **)malloc(sizeof(t_hmap *));
	if (!hashmap)
		exit(EXIT_FAILURE);
	*hashmap = NULL;
	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] != 0 && env[i][j] != '=')
			j++;
		hash_key = malloc(j + 1);
		if (!hash_key)
			exit(1);
		// ft_
		strncpy(hash_key, env[i], j);
		add_new_var(hashmap, hash_key, ft_strchr(env[i], '=') + 1);
		free(hash_key);
		i++;
	}
	return (hashmap);
}
int	key_exists(t_hmap *v, char *ekey)
{
	while (v)
	{
		// ft_
		if (strcmp(v->key, ekey) == 0)
			return (1);
		v = v->next;
	}
	return (0);
}

char	*return_value_hash(t_hmap *v, char *key)
{
	if (key_exists(v, key) == 1)
	{
		while (v)
		{
			// ft_
			if (strcmp(v->key, key) == 0)
				return (v->value);
			v = v->next;
		}
	}
	return (NULL);
}

static void get_paths(t_minishell *shell, t_cmd_data *cmd_data)
{
	char *paths_string;

	//down
	(void)shell;
	paths_string = return_value_hash(*(cmd_data->hashmap), "PATH");
	printf("paths_string: %s\n", paths_string);
	if (!paths_string)
	{
		cmd_data->cmd_path = NULL;
		return ;
	}
	cmd_data->cmd_paths = ft_split(paths_string, ':');
	printf("cmd_data->cmd_paths[0]: %s\n", cmd_data->cmd_paths[0]);
	printf("cmd_data->cmd_paths[1]: %s\n", cmd_data->cmd_paths[1]);
	if (!cmd_data->cmd_path)
	{
		perror("ft_split");
		exit(EXIT_FAILURE);
	}
}
void execution(t_minishell *shell, char **argv, t_cmd_data *cmd_data)
{
	get_paths(shell, cmd_data);

	if (is_builtin(argv[0]) == 1)
		exec_builtin(shell, argv);
	// else if (execve(cmd_data->cmd_path, argv, shell->env) == -1)
	// {
	// 	perror("execve");
	// 	exit(EXIT_FAILURE);
	// }
}

void	init_minishell(t_minishell *shell, char **envp, t_cmd_data *cmd_data)
{
	int		i;
	char	**argv;
	int		status;

	//char 	*cmdline = NULL;
	//char	*cmd_path = NULL;
	cmd_data->cmd_line = NULL;
	cmd_data->cmd_paths = NULL;
	cmd_data->cmd_path = NULL;
	cmd_data->hashmap = init_hmap(envp);

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
		ft_readline(&cmd_data->cmd_line, "minishell> ");
		if (cmd_data->cmd_line)
		{
			if (ft_strncmp(cmd_data->cmd_line, "exit", 4) == 0)
			{
				free(cmd_data->cmd_line);
				break ;
			}
			else
			{
			argv = ft_split(cmd_data->cmd_line, ' ');
			if (!argv)
			{
				free(cmd_data->cmd_line);
				continue ;
			}
			printf("argv[0]: %s\n", argv[0]);
			pid_t pid = fork();
			if (pid == 0)
			{
				// Child process
				execution(shell, argv, cmd_data);
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
			free(cmd_data->cmd_path);
			i = 0;
			while (argv[i])
				free(argv[i++]);
			free(argv);
			add_history(cmd_data->cmd_line);
			free(cmd_data->cmd_line);
			}
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	shell;
	t_cmd_data	cmd_data;

	if (argc < 1 || !argv)
		return (0);
	init_minishell(&shell, envp, &cmd_data);
	return (0);
}
