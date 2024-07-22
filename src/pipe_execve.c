#include "../minishell.h"

int check_if_executable(char *cmd)
{
	int res;

	res = access(cmd, X_OK | F_OK);
	if (res == 0)
	{
		// //printf("Executable\n");
		return (1);
	}
	return (0);
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
		return ;
	}
	//printf("Retrieved PATH: %s\n", paths_string); // Debugging print
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
		//printf("Trying path: %s\n", cmd_path); // Debugging print
		if (check_if_executable(cmd_path) == 1)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (NULL);
}

char **convert_hashmap(t_hmap *hashmap)
{
	int count;
	int i;
	size_t len;

	t_hmap *current = hashmap;
	// Count the number of environment variables
	count = 0;
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
	i = 0;
	while (current)
	{
		// Create the "KEY=VALUE" string
		len = strlen(current->key) + strlen(current->value) + 2; // +2 for '=' and '\0'
		env_array[i] = (char *)malloc(len * sizeof(char));
		if (!env_array[i])
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		// need to change to ft
		strcpy(env_array[i], current->key);
		strcat(env_array[i], "=");
		strcat(env_array[i], current->value);
		i++;
		current = current->next;
	}
	// NULL terminate the environment array
	env_array[count] = NULL;
	return env_array;
}

void execution(t_minishell *shell, char **argv, t_cmd_data *cmd_data)
{
	char	**env_array;
	////printf("Command to execute: %s\n", argv[0]);

	// Check if the command is a relative or absolute path
	if (argv[0][0] == '.' || argv[0][0] == '/')
	{
		cmd_data->cmd_path = argv[0];
	}
	else
	{
		// Otherwise, search for the command in the PATH directories
		get_paths(shell, cmd_data);
		if (cmd_data->cmd_paths)
		{
			cmd_data->cmd_path = get_cmd_path(cmd_data->cmd_paths, argv[0]);
			if (!cmd_data->cmd_path)
			{
				//printf("Command not found: %s\n", argv[0]);
				print_err_msg(argv[0], ": command not found\n");
				shell->exit_status = CMD_NF_FAILURE;
				exit (shell->exit_status);
				//return;
			}
		}
		else
			cmd_data->cmd_path = argv[0];
	}
	if (check_if_executable(cmd_data->cmd_path) != 1)
	{
		//printf("Command not found or not executable: %s\n", cmd_data->cmd_path);
			print_err_msg(argv[0], ": No such file or directory\n");
			shell->exit_status = CMD_NF_FAILURE;
			exit (shell->exit_status);
	}
	env_array = convert_hashmap(*(shell->hashmap));
	if (execve(cmd_data->cmd_path, argv, env_array) == -1)
	{
		perror("execve");
		exit(EXIT_FAILURE);
	}
}

// void execution(t_minishell *shell, char **argv, t_cmd_data *cmd_data)
// {
// 	char	**env_array;

// 	get_paths(shell, cmd_data);
// 	if (cmd_data->cmd_paths)
// 	{
// 		cmd_data->cmd_path = get_cmd_path(cmd_data->cmd_paths, argv[0]);
// 		//printf("PLEASE TAKE A LOOK HERE: %s", cmd_data->cmd_path);
// 		if (!cmd_data->cmd_path)
// 		{
// 			//printf("Command not found: %s\n", argv[0]);
// 			shell->exit_status = 127;
// 			return;
// 		}
// 	}
// 	env_array = convert_hashmap(*(shell->hashmap));
// 	if (execve(cmd_data->cmd_path, argv, env_array) == -1)
// 	{
// 		perror("execve");
// 		exit(EXIT_FAILURE);
// 	}
// }