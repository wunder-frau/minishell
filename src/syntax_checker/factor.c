/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   factor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 01:34:52 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/21 01:51:41 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*syntax_checker_redirect(char *str, bool *status)
{
	char	*next_token;

	if (ft_strncmp(">>", str, 2) == 0)
		str += 2;
	else if (ft_strncmp("<<", str, 2) == 0)
		str += 2;
	else if (ft_strncmp(">", str, 1) == 0)
		str += 1;
	else if (ft_strncmp("<", str, 1) == 0)
		str += 1;
	else
		return (str);
	while (ft_is_space(*str))
		str++;
	next_token = syntax_checker_word(str, status);
	if (next_token == str)
		*status = false;
	if (*status == false)
		return (next_token);
	while (ft_is_space(*next_token))
		next_token++;
	next_token = syntax_checker_redirect(next_token, status);
	return (next_token);
}

char	*syntax_checker_word(char *str, bool *status)
{
	char	*special_characters;

	special_characters = "|<> '\"";
	while (*str && ft_strchr(special_characters, *str) == NULL)
		str++;
	if (*str == S_QUO || *str == D_QUO)
		str = handle_quoted_string(str, status);
	return (str);
}

char	*handle_quoted_string(char *str, bool *status)
{
	char	quote_char;

	quote_char = *str;
	str++;
	while (*str && *str != quote_char)
		str++;
	if (*str == quote_char)
		str++;
	else
		*status = false;
	return (str);
}
