/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 00:38:28 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/26 02:05:31 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_minishell(t_minishell **shell, char **envp, t_cmd_data *cmd_data)
{
	int	status;

	status = 0;
	*shell = ft_calloc(1, sizeof(t_minishell));
	if (!*shell)
		status = 200;
	if (status == 0)
	{
		cmd_data->hashmap = init_hmap(envp);
		if (!cmd_data->hashmap)
			status = 200;
	}
	(*shell)->hashmap = cmd_data->hashmap;
	(*shell)->cmd_data = *cmd_data;
	(*shell)->is_parent = true;
	add_shlvl(*shell);
	(*shell)->is_parent = true;
	g_sigint_received = 0;
	if (status != 0)
		terminate_minishell(shell, status);
}

void	terminate_minishell(t_minishell **shell, int status)
{
	free_minishell(*shell);
	ft_putstr_fd("Error: Failed to initialize t_minishell"
		"structure\n", STDERR_FILENO);
	exit (status);
}
