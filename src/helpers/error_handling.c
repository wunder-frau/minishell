/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 00:11:20 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/23 10:17:54 by istasheu         ###   ########.fr       */
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