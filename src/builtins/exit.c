/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 00:42:31 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/22 00:35:42 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_multiple_args(char **arg, t_minishell *shell)
{
	long	num;

	num = ft_atol(arg[0]);
	if ((is_non_empty_after_trim(arg[0]) && num == 0)
		|| is_non_digit_space_sign(arg[0]))
		exit_numeric_arg_error(shell, arg[0]);
	else
		exit_amount_of_arg_error(shell);
}

void	handle_single_arg(char *arg, t_minishell *shell)
{
	long	num;

	num = ft_atol(arg);
	if ((is_non_empty_after_trim(arg) && num == 0)
		|| is_non_digit_space_sign(arg))
		exit_numeric_arg_error(shell, arg);
	else
		shell->exit_status = (unsigned char)num;
}

void	ft_exit(char **arg, t_minishell *shell)
{
	bool	is_multiple_args;
	bool	is_empty_args;

	is_multiple_args = (ft_size((void **)arg) > 1);
	is_empty_args = !*arg;
	if (is_multiple_args)
		handle_multiple_args(arg, shell);
	else if (is_empty_args)
		shell->exit_status = SUCCESS;
	else
		handle_single_arg(arg[0], shell);
	if (shell->is_parent == true)
		ft_putstr_fd("exit\n", STDERR_FILENO);
	if (shell->is_parent == true)
		clean_and_exit(shell);
	exit(shell->exit_status);
}
