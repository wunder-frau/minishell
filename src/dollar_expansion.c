/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 16:17:00 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/07/24 16:24:12 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strdup(const char *s1)
{
	char	*copy;
	size_t	len;

	len = strlen(s1) + 1;
	copy = (char *)malloc(len);
	if (copy)
	{
		ft_memcpy(copy, s1, len);
	}
	return (copy);
}

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
	// need ft_strncpy
	strncpy(var_name, str, len);
	var_name[len] = '\0';
	*var_len = len + 1;
	return (var_name);
}

char	*get_status_string(int last_status)
{
	return (ft_itoa(last_status));
}

char	*expand_variable(const char *str, t_minishell *ms, \
	int last_status, int *var_len)
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

void	process_single_quote(char **result, int *in_single_quotes, char c)
{
	*in_single_quotes = !*in_single_quotes;
	append_char_to_result(result, c);
}

void	process_double_quote(char **result, int *in_double_quotes, char c)
{
	*in_double_quotes = !*in_double_quotes;
	append_char_to_result(result, c);
}

void	process_dollar(char **result, char **str, int *i, t_minishell *ms, int last_status)
{
	int		var_len;
	char	*temp;
	char	*old_result;

	temp = expand_variable(*str + *i, ms, last_status, &var_len);
	if (!temp)
		return;
	old_result = *result;
	*result = ft_strjoin(*result, temp);
	free(old_result);
	free(temp);
	*i += var_len;
}

void	process_backslash_dollar(char **result, int *i)
{
	append_char_to_result(result, '$');
	*i += 2;
}

void	process_other_chars(char **result, char **str, int *i)
{
	append_char_to_result(result, (*str)[*i]);
	(*i)++;
}

void	process_quotes(char **result, int *in_quotes, char **str, int *i, \
	t_minishell *ms, int last_status)
{
	char	quote_char;

	quote_char = (*str)[*i];
	*in_quotes = !*in_quotes;
	append_char_to_result(result, quote_char);
	(*i)++;
	while ((*str)[*i] && (*str)[*i] != quote_char)
	{
		if ((*str)[*i] == '$' && quote_char == '\"')
			process_dollar(result, str, i, ms, last_status);
		else
		{
			append_char_to_result(result, (*str)[*i]);
			(*i)++;
		}
	}
	append_char_to_result(result, quote_char);
	(*i)++;
	*in_quotes = !*in_quotes;
}

int	dollar_expansion(char **str, t_minishell *ms, int last_status)
{
	char	*result;
	int		i;
	int		in_single_quotes;
	int		in_double_quotes;

	result = ft_strdup("");
	if (!result)
		return (-1);
	i = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	while ((*str)[i])
	{
		if ((*str)[i] == '\'')
			process_single_quote(&result, &in_single_quotes, (*str)[i++]);
		else if ((*str)[i] == '\"')
			process_double_quote(&result, &in_double_quotes, (*str)[i++]);
		else if ((*str)[i] == '\\' && (*str)[i + 1] == '$')
			process_backslash_dollar(&result, &i);
		else if ((*str)[i] == '$' && (in_double_quotes || (!in_single_quotes && !in_double_quotes)))
			process_dollar(&result, str, &i, ms, last_status);
		else
			process_other_chars(&result, str, &i);
	}
	free(*str);
	*str = result;
	return (0);
}
