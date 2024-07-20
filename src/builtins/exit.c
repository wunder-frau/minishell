/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 00:42:31 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/21 00:42:32 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_multiple_args(char **arg, t_minishell *ms)
{
	long	num;

	num = ft_atol(arg[0]);
	if ((is_non_empty_after_trim(arg[0]) && num == 0)
		|| is_non_digit_space_sign(arg[0]))
		exit_numeric_arg_error(ms, arg[0]);
	else
		exit_amount_of_arg_error(ms);
}

void	handle_single_arg(char *arg, t_minishell *ms)
{
	long	num;

	num = ft_atol(arg);
	if ((is_non_empty_after_trim(arg) && num == 0)
		|| is_non_digit_space_sign(arg))
		exit_numeric_arg_error(ms, arg);
	else
		ms->exit_status = (unsigned char)num;
}

void	ft_exit(char **arg, t_minishell *ms)
{
	bool	is_multiple_args;
	bool	is_empty_args;

	is_multiple_args = (ft_size((void **)arg) > 1);
	is_empty_args = !*arg;
	if (is_multiple_args)
		handle_multiple_args(arg, ms);
	else if (is_empty_args)
		ms->exit_status = SUCCESS;
	else
		handle_single_arg(arg[0], ms);
	if (ms->is_parent == true)
		ft_putstr_fd("exit\n", STDERR_FILENO);
	if (ms->is_parent == true)
		clean_and_exit(ms);
	exit(ms->exit_status);
}
