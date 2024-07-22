/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 00:11:20 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/22 08:38:03 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_err_msg(char *cmd, char *shellg)
{
	cmd = ft_strjoin("shell: ", cmd);
	if (!cmd)
		return ;
	shellg = ft_strjoin(cmd, shellg);
	free(cmd);
	if (!shellg)
		return ;
	ft_putstr_fd(shellg, STDERR_FILENO);
	free(shellg);
}

void	perror_err_shellg(char *cmd, char *arg)
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

void	print_arg_err_shellg(char *cmd, char *arg, char *shellg)
{
	cmd = ft_strjoin("shell: ", cmd);
	if (!cmd)
		return ;
	arg = ft_strjoin(cmd, arg);
	free(cmd);
	if (!arg)
		return ;
	shellg = ft_strjoin(arg, shellg);
	free(arg);
	if (!shellg)
		return ;
	ft_putstr_fd(shellg, STDERR_FILENO);
	free(shellg);
}
