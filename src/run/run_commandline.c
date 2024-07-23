/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_commandline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 01:55:49 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/23 01:24:41 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_readline(char **cmdline, char *prompt)
{
	*cmdline = readline(prompt);
}

void	run_commandline(t_minishell **shell)
{
	int		status;
	char	*cmdline = NULL;
	int		hd_c;

	while (1)
	{
		ft_readline(&cmdline, "minishell> ");
		if (ctrl_d_handler(cmdline) == 0)
		{
			free(cmdline);
			break ;
		}
		if (is_blank_string(cmdline))
		{
			free(cmdline);
			continue ;
		}
		if (cmdline[0] != '\0' && cmdline[0] != '\n')
			add_history(cmdline);
		status = syntax_checker_expression(cmdline);
		if (status != SUCCESS)
		{
			free(cmdline);
			continue ;
		}
		hd_c = 0;
		(*shell)->root = NULL;
		status = build_ast(cmdline, &((*shell)->root), &hd_c, *shell);
		if (status != 0)
			(*shell)->exit_status = status;
		if (status == 0)
			(*shell)->exit_status = traverse_tree(&((*shell)->root), *shell);
		free(cmdline);
		cmdline = NULL;
	}
}
