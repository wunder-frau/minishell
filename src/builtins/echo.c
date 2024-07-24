/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 16:30:31 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/07/24 16:31:22 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_echo(char **cmd)
{
	int		i;
	char	*temp;
	int		n_flag;

	i = 1;
	if (cmd[i] && ft_strcmp(cmd[i], "-n") == 0)
	{
		n_flag = 1;
		i++;
	}
	else
		n_flag = 0;
	while (cmd[i])
	{
		temp = ft_strdup(cmd[i]);
		ft_putstr_fd(temp, 1);
		free(temp);

		if (cmd[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (!n_flag)
		ft_putstr_fd("\n", 1);
}
