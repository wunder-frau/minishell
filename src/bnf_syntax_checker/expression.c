/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expression.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 01:31:49 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/21 01:31:53 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief    BNF form looks as follows:
 *           - <input>      ::= <pipeline> <newline>
 *           - <pipeline>   ::= <command> { '|' <command> }
 *           - <command>    ::= { <redirect> | <word> | "any" | 'any' }
 *           - <redirect>   ::= '<' | '>' | '<<' | '>>' <word>
 * @param    str String obtained from the readline function.
 * @return   -1 if a memory allocation error occurs during processing,
 *           1 if the input string passes syntax validation,
 *           0 if the input string fails to meet the required conditions.
 */

int	syntax_checker_expression(char *str)
{
	char	*next_token;
	bool	status;

	status = true;
	while (ft_is_space(*str))
		str++;
	if (*str == NULL_TERM)
		return (SUCCESS);
	next_token = syntax_checker_pipeline(str, &status);
	if (*next_token != NULL_TERM || status == false)
	{
		handle_syntax_error(next_token);
		return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}

char	*syntax_checker_pipeline(char *str, bool *status)
{
	char	*next_token;

	while (ft_is_space(*str))
		str++;
	next_token = syntax_checker_command(str, status);
	if (next_token == str)
		*status = false;
	if (*status == false)
		return (next_token);
	else if (*next_token == PI && next_token[1] != PI)
	{
		next_token++;
		if (is_blank_string(next_token) == true)
		{
			*status = false;
			return (next_token);
		}
		next_token = syntax_checker_pipeline(next_token, status);
	}
	return (next_token);
}
