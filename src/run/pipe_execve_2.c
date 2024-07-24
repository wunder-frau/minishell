/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execve_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 18:23:05 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/07/24 18:34:21 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_if_executable(char *cmd)
{
	struct stat	st;
	DIR			*dir;

	if (access(cmd, F_OK) != 0)
	{
		return (CMD_NF_FAILURE);
	}
	if (stat(cmd, &st) != 0)
	{
		perror("stat");
		return (CMD_NF_FAILURE);
	}
	if (access(cmd, X_OK) != 0)
	{
		return (CMD_PD_FAILURE);
	}
	dir = opendir(cmd);
	if (dir)
	{
		closedir(dir);
		return (CMD_PD_FAILURE);
	}
	return (0);
}

char	**convert_hashmap(t_hmap *hashmap)
{
	int		count;
	char	**env_array;

	count = count_hashmap_entries(hashmap);
	env_array = allocate_env_array(count);
	fill_env_array(env_array, hashmap);
	return (env_array);
}

char	*get_cmd_path(char **cmd_paths, char *cmd)
{
	int		i;
	char	*cmd_path;
	char	*temp_path;

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
		if (check_if_executable(cmd_path) == 0)
		{
			return (cmd_path);
		}
		free(cmd_path);
		i++;
	}
	return (NULL);
}

char	**allocate_env_array(int count)
{
	char	**env_array;

	env_array = (char **)malloc((count + 1) * sizeof(char *));
	if (!env_array)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return (env_array);
}

int	count_hashmap_entries(t_hmap *hashmap)
{
	int		count;
	t_hmap	*current;

	count = 0;
	current = hashmap;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}
