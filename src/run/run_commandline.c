/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_commandline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 18:33:10 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/07/24 18:36:05 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_readline(char **cmdline, char *prompt)
{
	*cmdline = readline(prompt);
}

void	process_command(t_minishell **shell, char *cmdline)
{
	int		status;
	int		hd_c;

	if (cmdline[0] != '\0' && cmdline[0] != '\n')
		add_history(cmdline);
	dollar_expansion(&cmdline, *shell, (*shell)->exit_status);
	status = syntax_checker_expression(cmdline);
	if (status != SUCCESS)
	{
		free(cmdline);
		return ;
	}
	hd_c = 0;
	(*shell)->root = NULL;
	status = build_ast(cmdline, &((*shell)->root), &hd_c, *shell);
	if (status != 0 && cmdline[0] != '\0' && cmdline[0] != '\n')
		(*shell)->exit_status = status;
	if (status == 0)
		(*shell)->exit_status = traverse_tree(&((*shell)->root), *shell);
	free(cmdline);
}

void	run_commandline(t_minishell **shell)
{
	char	*cmdline;

	cmdline = NULL;
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
		process_command(shell, cmdline);
		cmdline = NULL;
	}
}
