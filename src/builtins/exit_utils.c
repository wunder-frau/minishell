/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 00:43:58 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/23 01:52:12 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_non_digit_space_sign(const char *str)
{
	while (*str)
	{
		if (!ft_isdigit(*str) && !ft_is_space(*str)
			&& *str != '+' && *str != '-')
			return (true);
		str++;
	}
	return (false);
}

bool	is_non_empty_after_trim(const char *str)
{
	int	i;

	i = 0;
	while (ft_is_space(*str))
		str++;
	if ((*str == '+' || *str == '-') && ft_isdigit(str[i + 1]))
		str++;
	while (*str == '0')
		str++;
	while (ft_is_space(*str))
		str++;
	return (*str != '\0');
}

void	exit_numeric_arg_error(t_minishell *shell, char *str)
{
	// if (shell->is_parent)
	// 	ft_putstr_fd("exit\n", STDERR_FILENO);
	(void)str;
	ft_putstr_fd("minishell: cd: numeric argument required\n", 2);
	shell->exit_status = CMD_ARG_ERROR;
	if (shell->is_parent == true)
		clean_and_exit(shell);
	exit(shell->exit_status);
}

void	exit_amount_of_arg_error(t_minishell *shell)
{
	// if (shell->is_parent)
	// 	ft_putstr_fd("exit\n", STDERR_FILENO);
	ft_putstr_fd("minishell: cd: too many arguments\n", 2);
	shell->exit_status = GENERIC_ERROR;
}

void	clean_and_exit(t_minishell *shell)
{
	int	status;

	status = shell->exit_status;
	rl_clear_history();
	free_minishell(shell);
	exit(status);
}
