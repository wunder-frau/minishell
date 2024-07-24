/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execve.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:59:09 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/07/24 18:28:37 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	fill_env_array(char **env_array, t_hmap *hashmap)
{
	int		i;
	size_t	len;
	t_hmap	*current;

	i = 0;
	current = hashmap;
	while (current)
	{
		len = strlen(current->key) + strlen(current->value) + 2;
		env_array[i] = (char *)malloc(len * sizeof(char));
		if (!env_array[i])
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		strcpy(env_array[i], current->key);
		strcat(env_array[i], "=");
		strcat(env_array[i], current->value);
		i++;
		current = current->next;
	}
	env_array[i] = NULL;
}

static void	get_paths(t_minishell *shell, t_cmd_data *cmd_data)
{
	char	*paths_string;

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
	cmd_data->cmd_paths = ft_split(paths_string, ':');
	if (!cmd_data->cmd_paths)
	{
		perror("ft_split");
		exit(EXIT_FAILURE);
	}
}

void	handle_cmd_path(t_minishell *shell, char **argv, t_cmd_data *cmd_data)
{
	if (argv[0][0] == '.' || argv[0][0] == '/')
		cmd_data->cmd_path = argv[0];
	else
	{
		get_paths(shell, cmd_data);
		if (cmd_data->cmd_paths)
		{
			cmd_data->cmd_path = get_cmd_path(cmd_data->cmd_paths, argv[0]);
			if (!cmd_data->cmd_path)
			{
				print_err_msg(argv[0], ": command not found\n");
				shell->exit_status = CMD_NF_FAILURE;
				exit(shell->exit_status);
			}
		}
		else
			cmd_data->cmd_path = argv[0];
	}
}

void	handle_exec_status(t_minishell *shell, t_cmd_data *cmd_data)
{
	int				exec_status;
	DIR				*dir;

	exec_status = check_if_executable(cmd_data->cmd_path);
	if (exec_status != 0)
	{
		if (exec_status == CMD_NF_FAILURE)
			print_err_msg(cmd_data->cmd_path, ": No such file or directory\n");
		else if (exec_status == CMD_PD_FAILURE)
		{
			dir = opendir(cmd_data->cmd_path);
			if (dir)
			{
				print_err_msg(cmd_data->cmd_path, ": is a directory\n");
				closedir(dir);
			}
			else
				print_err_msg(cmd_data->cmd_path, ": Permission denied\n");
		}
		shell->exit_status = exec_status;
		exit(shell->exit_status);
	}
}

void	execution(t_minishell *shell, char **argv, t_cmd_data *cmd_data)
{
	char			**env_array;

	handle_cmd_path(shell, argv, cmd_data);
	handle_exec_status(shell, cmd_data);
	env_array = convert_hashmap(*(shell->hashmap));
	if (execve(cmd_data->cmd_path, argv, env_array) == -1)
	{
		perror("execve");
		shell->exit_status = CMD_NF_FAILURE;
		exit(EXIT_FAILURE);
	}
}
