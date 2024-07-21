/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 01:45:11 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/21 01:45:13 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*syntax_checker_command(char *str, bool *status)
{
	char	*next_token;

	while (ft_is_space(*str))
		str++;
	next_token = syntax_checker_redirect(str, status);
	if (next_token == str)
		next_token = syntax_checker_word(str, status);
	if (*status == false)
		return (next_token);
	if (next_token != str)
		next_token = syntax_checker_command(next_token, status);
	return (next_token);
}
