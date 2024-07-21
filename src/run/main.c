/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 00:18:40 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/22 01:47:31 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_minishell		*shell;
	t_cmd_data		cmd_data;

	if (argc < 1 || !argv)
		return (0);
	set_signals();
	init_minishell(&shell, envp, &cmd_data);
	run_commandline(&shell);
	rl_clear_history();
	free_minishell(shell);
	shell = NULL;
	return (0);
}
