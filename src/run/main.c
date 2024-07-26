/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 00:18:40 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/26 17:37:16 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile sig_atomic_t	g_sigint_received = 0;

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
