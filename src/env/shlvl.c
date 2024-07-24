/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:16:54 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/07/24 17:17:26 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_shlvl(t_minishell *shell)
{
	char	*shlvl;
	char	*value;
	int		number;

	shlvl = ft_get_env(*(shell->hashmap), "SHLVL");
	if (shlvl)
	{
		number = ft_atoi(shlvl);
		number++;
		value = ft_itoa(number);
		ft_add_env_hash(shell->hashmap, "SHLVL", value);
		free(value);
	}
	else
	{
		ft_add_env_hash(shell->hashmap, "SHLVL", "1");
	}
}
