/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 23:39:39 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/21 23:39:40 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_inside_quotes(char c, int *inside_quotes)
{
	if ((c == S_QUO || c == D_QUO) && *inside_quotes == 0)
	{
		*inside_quotes = c;
		return (true);
	}
	else if (c == S_QUO && *inside_quotes == S_QUO)
	{
		*inside_quotes = 0;
		return (true);
	}
	else if (c == D_QUO && *inside_quotes == D_QUO)
	{
		*inside_quotes = 0;
		return (true);
	}
	return (false);
}

void	remove_quotes(char *str, int i, int j)
{
	int	len;
	int	inside_quotes;

	len = ft_strlen(str);
	inside_quotes = 0;
	while (i < len)
	{
		if (!is_inside_quotes(str[i], &inside_quotes))
			str[j++] = str[i];
		i++;
	}
	str[j] = NULL_TERM;
}

void	remove_quotes_arr(char **arr, int i)
{
	while (arr[i])
	{
		remove_quotes(arr[i], 0, 0);
		i++;
	}
}

static int	skip_quotes(char *str)
{
	int	i;

	i = 0;
	if (*str == D_QUO)
	{
		i++;
		while (str[i] != D_QUO)
			i++;
	}
	else if (*str == S_QUO)
	{
		i++;
		while (str[i] != S_QUO)
			i++;
	}
	return (i);
}

char	**split_handle_quotes_and_spaces(char *str)
{
	char	**res;
	int		i;

	i = 0;
	while (str[i] != NULL_TERM)
	{
		if (ft_is_space(str[i]))
			str[i] = SEPARATOR;
		else if (str[i] == D_QUO || str[i] == S_QUO)
			i += skip_quotes(str + i);
		i++;
	}
	res = ft_split(str, SEPARATOR);
	if (!res)
		return (NULL);
	i = 0;
	while (res[i])
		i++;
	return (res);
}
