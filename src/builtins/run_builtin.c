/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 22:59:44 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/22 00:35:42 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	run_builtin(char **redir, t_minishell *shell)
{
	int	status;

	status = 0;
	if (!redir)
		status = exec_builtin(shell);
	else
		status = exec_builtin_redir(shell->command, redir, shell);
	return (status);
}

int	exec_builtin_redir(char **command, char **redir, t_minishell *shell)
{
	int	status;
	int	in_fd;
	int	out_fd;

	status = copy_std_fd(&in_fd, &out_fd, command[0]);
	if (status == 0)
		status = apply_redirects(redir, shell);
	if (status == 0)
		status = exec_builtin(shell);
	return_std_fd(&in_fd, &out_fd, &status, command[0]);
	return (status);
}

int	copy_std_fd(int *in_fd, int *out_fd, char *command)
{
	*in_fd = dup(STDIN_FILENO);
	*out_fd = dup(STDOUT_FILENO);
	if (*in_fd == -1 || *out_fd == -1)
	{
		if (*in_fd != -1)
			close(*in_fd);
		if (*out_fd != -1)
			close(*out_fd);
		print_err_shellg(command, ": dup() error occurred\n");
		return (DUP_FAILURE);
	}
	return (0);
}

void	return_std_fd(int *in_fd, int *out_fd, int *status, char *command)
{
	int	redir_status;

	redir_status = 0;
	if (dup2(*in_fd, STDIN_FILENO) == -1 || dup2(*out_fd, STDOUT_FILENO) == -1)
	{
		print_err_shellg(command, ": dup2() error occured. "
			"Correct behavior is not guaranteed anymore\n");
		redir_status = DUP_FAILURE;
	}
	close(*in_fd);
	close(*out_fd);
	if (*status == 0)
		*status = redir_status;
}
