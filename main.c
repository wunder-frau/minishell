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

void ft_readline(char **cmdline, char *prompt)
{
	*cmdline = readline(prompt);
}

int check_if_executable(char *cmd)
{
	int res;

	res = access(cmd, X_OK | F_OK);
	if (res == 0)
	{
		printf("Executable\n");
		return (1);
	}
	return (0);
}

static t_hmap *hmap_new(char *akey, char *avalue)
{
	t_hmap *node;

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

void add_new_var(t_hmap **v, char *akey, char *avalue)
{
	t_hmap *node;
	t_hmap *temp;

	if (akey[0] == ' ')
		node = hmap_new(akey + 1, NULL);
	else
		node = hmap_new(akey, avalue);
	if (!node)
		return;
	if (*v == NULL)
		*v = node;
	else
	{
		temp = *v;
		while (temp->next)
			temp = temp->next;
		temp->next = node;
	}
	printf("Added key: %s, value: %s\n", akey, avalue);
}

void free_hashmap(t_hmap *hashmap)
{
    t_hmap *current;
    t_hmap *next;

    current = hashmap;
    while (current)
	{
        next = current->next;
        free(current->key);
        free(current->value);
        free(current);
        current = next;
    }
}

t_hmap **init_hmap(char **env)
{
	char *hash_key;
	t_hmap **hashmap;
	int j;
	int i;

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
		strncpy(hash_key, env[i], j);
		hash_key[j] = '\0';
		add_new_var(hashmap, hash_key, ft_strchr(env[i], '=') + 1);
		free(hash_key);
		i++;
	}
	return (hashmap);
}

int key_exists(t_hmap *v, char *ekey)
{
	while (v)
	{
		if (strcmp(v->key, ekey) == 0)
			return (1);
		v = v->next;
	}
	return (0);
}

char *return_value_hash(t_hmap *v, char *key)
{
	if (key_exists(v, key) == 1)
	{
		while (v)
		{
			if (strcmp(v->key, key) == 0)
			{
				printf("Found key: %s, value: %s\n", key, v->value);
				return (v->value);
			}
			v = v->next;
		}
	}
	printf("Key not found: %s\n", key);
	return (NULL);
}

static void get_paths(t_minishell *shell, t_cmd_data *cmd_data)
{
	char *paths_string;

	if (!shell)
	{
		perror("shell");
		exit(EXIT_FAILURE);
	}
	if (!cmd_data)
	{
		perror("cmd_data");
		exit(EXIT_FAILURE);
	}
	paths_string = return_value_hash(*(cmd_data->hashmap), "PATH");
	if (!paths_string)
	{
		cmd_data->cmd_path = NULL;
		cmd_data->cmd_paths = NULL;
		return;
	}
	// printf("Retrieved PATH: %s\n", paths_string); // Debugging print
	cmd_data->cmd_paths = ft_split(paths_string, ':');
	if (!cmd_data->cmd_paths)
	{
		perror("ft_split");
		exit(EXIT_FAILURE);
	}
}

char *get_cmd_path(char **cmd_paths, char *cmd)
{
	int i;
	char *cmd_path;
	char *temp_path;

	i = 0;
	while (cmd_paths[i])
	{
		temp_path = ft_strjoin(cmd_paths[i], "/");
		if (!temp_path)
			return (NULL);
		cmd_path = ft_strjoin(temp_path, cmd);
		free(temp_path);
		if (!cmd_path)
			return (NULL);
		printf("Trying path: %s\n", cmd_path); // Debugging print
		if (check_if_executable(cmd_path) == 1)
		{
			return (cmd_path);
		}
		free(cmd_path);
		i++;
	}
	return (NULL);
}

char **convert_hashmap_to_env_array(t_hmap *hashmap)
{
	int count = 0;
	t_hmap *current = hashmap;

	// Count the number of environment variables
	while (current)
	{
		count++;
		current = current->next;
	}

	// Allocate memory for the environment array
	char **env_array = (char **)malloc((count + 1) * sizeof(char *));
	if (!env_array)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	// Populate the environment array
	current = hashmap;
	int i = 0;
	while (current)
	{
		// Create the "KEY=VALUE" string
		size_t len = ft_strlen(current->key) + ft_strlen(current->value) + 2; // +2 for '=' and '\0'
		env_array[i] = (char *)malloc(len * sizeof(char));
		if (!env_array[i])
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		snprintf(env_array[i], len, "%s=%s", current->key, current->value);
		i++;
		current = current->next;
	}
	// NULL terminate the environment array
	env_array[count] = NULL;
	return env_array;
}

void execution(t_minishell *shell, char **argv, t_cmd_data *cmd_data)
{
	get_paths(shell, cmd_data);
	if (cmd_data->cmd_paths)
	{
		cmd_data->cmd_path = get_cmd_path(cmd_data->cmd_paths, argv[0]);
		printf("Command path: %s\n", cmd_data->cmd_path); // Debugging print
		if (!cmd_data->cmd_path)
		{
			printf("Command not found: %s\n", argv[0]);
			shell->exit_status = 127;
			return;
		}
	}
	if (execve(cmd_data->cmd_path, argv, convert_hashmap_to_env_array(*(shell->hashmap))) == -1)
	{
		perror("execve");
		exit(EXIT_FAILURE);
	}
}

void init_minishell(t_minishell *shell, char **envp, t_cmd_data *cmd_data)
{
	int i;
	char **argv;
	int status;

	cmd_data->cmd_line = NULL;
	cmd_data->cmd_paths = NULL;
	cmd_data->cmd_path = NULL;

	cmd_data->hashmap = init_hmap(envp);
	shell->hashmap = (t_hmap **)malloc(sizeof(t_hmap *));
	if (!shell->hashmap)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	*(shell->hashmap) = *(cmd_data->hashmap);
	shell->oldpwd = NULL;
	shell->history = NULL;
	shell->history_path = NULL;
	shell->exit_status = 0;
	shell->root = NULL;
	shell->is_parent = 1;
	shell->is_oldpwd_unset = 1;
	shell->fd_output = 1;
	shell->cmd_data = cmd_data;
	while (1)
	{
		ft_readline(&cmd_data->cmd_line, "minishell> ");
		if (cmd_data->cmd_line)
		{
			if (ft_strncmp(cmd_data->cmd_line, "exit", 4) == 0)
			{
				free(cmd_data->cmd_line);
				break;
			}
			else
			{
				// printf("IN WHILE(1) shell->hashmap PWD with ft_get_env: %s\n", ft_get_env(*(shell->hashmap), "PWD"));
				argv = ft_split(cmd_data->cmd_line, ' ');
				if (!argv)
				{
					free(cmd_data->cmd_line);
					continue;
				}
				if (is_builtin(argv[0]) == 1)
				{
	  				printf("Executing builtin: %s\n", argv[0]);
					exec_builtin(shell, argv);
				}
				else
				{
					pid_t pid = fork();
					if (pid == 0)
					{
						// Child process
						printf("Executing command in child: %s\n", argv[0]);
						execution(shell, argv, cmd_data);
						exit(shell->exit_status);
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
				}
				i = 0;
				while (argv[i])
					free(argv[i++]);
				free(argv);
				add_history(cmd_data->cmd_line);
				free(cmd_data->cmd_line);
				//free_hashmap(*(shell->hashmap));
			}
		}
	}
}

int main(int argc, char **argv, char **envp)
{
	t_minishell shell;
	t_cmd_data cmd_data;

	if (argc < 1 || !argv)
		return (0);
	init_minishell(&shell, envp, &cmd_data);
	return (0);
}
