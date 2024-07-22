/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_syntax_error.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 01:29:43 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/22 07:56:03 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_blank_string(char *str)
{
	if (ft_strlen(str) == 0)
		return (true);
	while (ft_is_space(*str))
		str++;
	return (*str == NULL_TERM);
}

bool	is_alnum_or_quote(char c)
{
	return (ft_isalnum(c) || c == S_QUO || c == D_QUO);
}

void	print_error_message(char *str)
{
	ft_putstr_fd("shell: syntax error near unexpected token `", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
}

bool	is_special_token(char *str, int *length)
{
	if (strncmp(">>", str, 2) == 0 || strncmp("<<", str, 2) == 0)
	{
		*length = 2;
		return (true);
	}
	else if (*str == PI || *str == REDIR_L || *str == REDIR_R)
	{
		*length = 1;
		return (true);
	}
	return (false);
}

void	handle_syntax_error(char *str)
{
	int	i;
	int	token_length;

	i = 0;
	token_length = 0;
	if (*str == NULL_TERM)
		str = "newline";
	else if (is_special_token(str, &token_length))
		i = token_length;
	else if (is_alnum_or_quote(*str))
		while (is_alnum_or_quote(str[i]))
			i++;
	if (i != 0)
		str[i] = NULL_TERM;
	print_error_message(str);
}
