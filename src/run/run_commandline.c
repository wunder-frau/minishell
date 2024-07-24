/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_commandline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 01:55:49 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/24 11:39:01 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_readline(char **cmdline, char *prompt)
{
	signal_interceptor(1);
	set_signals(0);
	*cmdline = readline(prompt);
	if (!ctrl_d_handler(*cmdline)) // Handle Ctrl+D
    {
        free(*cmdline);
        *cmdline = NULL;
        return;
    }
	signal_interceptor(3);
}

static void	check_signal(t_minishell **shell)
{
	if (g_sigint_received == SIGINT)
	{
		(*shell)->exit_status = 1;
		g_sigint_received = 0;
	}
}

void	run_commandline(t_minishell **shell)
{
	int		status;
	char	*cmdline;
	int		hd_c;
	while (1)
	{
		status = get_cmdline(&cmdline, shell);
		if (status != 0 && status != EOF)
			(*shell)->exit_status = status;
		if (status == EOF || status == MALLOC_ERR)
			break ;
		if (status == 0)
		{
			hd_c = 0;
			(*shell)->root = NULL;
			status = build_ast(cmdline, &((*shell)->root), &hd_c, *shell);
			if (status != 0)
				(*shell)->exit_status = status;
			if (status == 0)
				(*shell)->exit_status = traverse_tree(&((*shell)->root), *shell);
			free(cmdline);
		}
	}
}

int	get_cmdline(char **cmdline, t_minishell **ms)
{
	int		status;

	status = 0;
	ft_readline(cmdline, "minishell> ");
	if (*cmdline == NULL)
	{
		if (ctrl_d_handler(*cmdline) == 0)
			{
				free(*cmdline);
				return (EOF);
			}
	}
	if (ft_strlen(*cmdline) == 0)
		{
			free(*cmdline);
			check_signal(ms);
			return (get_cmdline(cmdline, ms));
		}
	if (*cmdline[0] != '\0' && *cmdline[0] != '\n')
			add_history(*cmdline);
	status = syntax_checker_expression(*cmdline);
	if (status != 0)
		free(*cmdline);
	return (status);
}
