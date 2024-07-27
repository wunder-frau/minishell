/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 00:11:20 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/27 11:58:31 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_err_msg(char *cmd, char *msg)
{
	cmd = ft_strjoin("shell: ", cmd);
	if (!cmd)
		return ;
	msg = ft_strjoin(cmd, msg);
	free(cmd);
	if (!msg)
		return ;
	ft_putstr_fd(msg, STDERR_FILENO);
	free(msg);
}

void	perror_err_msg(char *cmd, char *arg)
{
	cmd = ft_strjoin("shell: ", cmd);
	if (!cmd)
		return ;
	arg = ft_strjoin(cmd, arg);
	free(cmd);
	if (!arg)
		return ;
	perror(arg);
	free(arg);
}

void	print_arg_err_msg(char *cmd, char *arg, char *msg)
{
	cmd = ft_strjoin("shell: ", cmd);
	if (!cmd)
		return ;
	arg = ft_strjoin(cmd, arg);
	free(cmd);
	if (!arg)
		return ;
	msg = ft_strjoin(arg, msg);
	free(arg);
	if (!msg)
		return ;
	ft_putstr_fd(msg, STDERR_FILENO);
	free(msg);
}

void	signal_status_handler(t_minishell **shell)
{
	if (g_sigint_received == SIGINT)
	{
		(*shell)->exit_status = 1;
		g_sigint_received = 0;
	}
}

void	show_sgnl_err_msg(int status)
{
	int	sigcode;

	sigcode = WTERMSIG(status);
	if (sigcode == SIGINT)
		ft_putstr_fd("\n", STDERR_FILENO);
	else if (sigcode == SIGQUIT)
		ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
	else if (sigcode == SIGABRT)
		ft_putstr_fd("Abort trap: 6\n", STDERR_FILENO);
	else if (sigcode == SIGKILL)
		ft_putstr_fd("Killed: 9\n", STDERR_FILENO);
	else if (sigcode == SIGBUS)
		ft_putstr_fd("Bus error: 10\n", STDERR_FILENO);
	else if (sigcode == SIGSEGV)
		ft_putstr_fd("Segmentation fault: 11\n", STDERR_FILENO);
	else if (sigcode == SIGPIPE)
		;
	else
		ft_putstr_fd("Unknown signal\n", STDERR_FILENO);
}
