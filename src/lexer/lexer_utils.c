/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 01:31:32 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/23 01:32:49 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	quote_tracker(char *str, int *i, int *quote_status)
{
	char	cur_char;

	cur_char = str[*i];
	if ((cur_char == S_QUO && *quote_status == S_QUO)
		|| (cur_char == D_QUO && *quote_status == D_QUO))
		*quote_status = 0;
	else if (cur_char == S_QUO || cur_char == D_QUO)
		*quote_status = cur_char;
}

static void	copy_and_replace_with_space(char *str, char *r, int *i, int *j)
{
	r[*j] = str[*i];
	str[*i] = SPA;
	(*j)++;
	(*i)++;
}

static int	is_special_char(char c)
{
	return (c == REDIR_L || c == REDIR_R || c == SPA || c == HT);
}

static void	extract_redirect(char *str, char **redir, int *i, int *j)
{
	int	quote_status;

	quote_status = 0;
	while (is_special_char(str[*i]) && str[*i] != NULL_TERM)
		copy_and_replace_with_space(str, *redir, i, j);
	while (str[*i] != NULL_TERM)
	{
		is_inside_quotes(str[*i], &quote_status);
		if (quote_status == 0 && (is_special_char(str[*i])
				|| str[*i] == NULL_TERM))
			break ;
		copy_and_replace_with_space(str, *redir, i, j);
	}
	(*redir)[(*j)++] = SEPARATOR;
	(*i)--;
}

int	convert_input_to_redirects(char *str, char **redir, int i,
	int j)
{
	int	quote_status;

	quote_status = 0;
	if (!str)
		return (-1);
	*redir = (char *)ft_calloc(ft_strlen(str) * 2, sizeof(char));
	if (!*redir)
		return (-1);
	while (str[i] != NULL_TERM)
	{
		quote_tracker(str, &i, &quote_status);
		if (quote_status == 0 && (str[i] == REDIR_L || str[i] == REDIR_R))
			extract_redirect(str, redir, &i, &j);
		if (str[i] == NULL_TERM)
			break ;
		i++;
	}
	return (1);
}
