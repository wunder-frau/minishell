/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_expansion_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 19:37:01 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/07/25 09:10:30 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_var_name(const char *str, int *var_len)
{
	int		len;
	char	*var_name;

	len = 0;
	while (str[len] && (str[len] != ' ' && str[len] != '$' \
	&& str[len] != '\'' && str[len] != '\"' && str[len] != ',' \
	&& str[len] != '\\' && str[len] != '\0'))
		len++;
	var_name = (char *)malloc(len + 1);
	if (!var_name)
		return (NULL);
	ft_strncpy(var_name, str, len);
	var_name[len] = '\0';
	*var_len = len + 1;
	return (var_name);
}

char	*get_status_string(int last_status)
{
	return (ft_itoa(last_status));
}

char	*expand_variable(const char *str, t_minishell *ms, int last_status, \
int *var_len)
{
	char	*value;
	char	*key;
	char	*status_str;

	if (str[1] == '\0' || str[1] == ' ' || str[1] == '\"')
	{
		*var_len = 1;
		return (ft_strdup("$"));
	}
	if (str[1] == '?')
	{
		status_str = get_status_string(last_status);
		*var_len = 2;
		return (status_str);
	}
	key = get_var_name(str + 1, var_len);
	if (!key)
		return (ft_strdup(""));
	value = ft_get_env(*(ms->hashmap), key);
	free(key);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

void	append_char_to_result(char **result, char c)
{
	char	append_str[2];
	char	*old_result;

	append_str[0] = c;
	append_str[1] = '\0';
	old_result = *result;
	*result = ft_strjoin(*result, append_str);
	free(old_result);
}
