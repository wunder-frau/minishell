/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 19:35:12 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/07/24 19:38:22 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	process_single_quotes(t_expansion_state *state)
{
	process_single_quote(state, (*(state->str))[*(state->i)]);
	(*(state->i))++;
	while ((*(state->str))[*(state->i)] && (*(state->str))[*(state->i)] != '\'')
	{
		append_char_to_result(state->result, (*(state->str))[*(state->i)]);
		(*(state->i))++;
	}
	if ((*(state->str))[*(state->i)] == '\'')
	{
		append_char_to_result(state->result, (*(state->str))[*(state->i)]);
		(*(state->i))++;
		*(state->in_single_quotes) = !*(state->in_single_quotes);
	}
}

void	process_double_quotes(t_expansion_state *state)
{
	process_double_quote(state, (*(state->str))[*(state->i)]);
	(*(state->i))++;
	while ((*(state->str))[*(state->i)] && (*(state->str))[*(state->i)] != '\"')
	{
		if ((*(state->str))[*(state->i)] == '$')
			process_dollar(state);
		else
			process_other_chars(state);
	}
	if ((*(state->str))[*(state->i)] == '\"')
	{
		append_char_to_result(state->result, (*(state->str))[*(state->i)]);
		(*(state->i))++;
		*(state->in_double_quotes) = !*(state->in_double_quotes);
	}
}

void	handle_quotes(t_expansion_state *state)
{
	if ((*(state->str))[*(state->i)] == '\'')
		process_single_quotes(state);
	else if ((*(state->str))[*(state->i)] == '\"')
		process_double_quotes(state);
}

void	process_expansion(t_expansion_state *state)
{
	while ((*(state->str))[*(state->i)])
	{
		if ((*(state->str))[*(state->i)] == '\'' || \
		(*(state->str))[*(state->i)] == '\"')
			handle_quotes(state);
		else if ((*(state->str))[*(state->i)] == '\\' && \
		(*(state->str))[*(state->i) + 1] == '$')
			process_backslash_dollar(state);
		else if ((*(state->str))[*(state->i)] == '$' && \
		(*(state->in_double_quotes) || (!*(state->in_single_quotes) \
		&& !*(state->in_double_quotes))))
			process_dollar(state);
		else
			process_other_chars(state);
	}
}

int	dollar_expansion(char **str, t_minishell *ms, int last_status)
{
	char				*result;
	int					i;
	int					in_single_quotes;
	int					in_double_quotes;
	t_expansion_state	state;

	result = ft_strdup("");
	if (!result)
		return (-1);
	i = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	state.result = &result;
	state.str = str;
	state.i = &i;
	state.ms = ms;
	state.last_status = last_status;
	state.in_single_quotes = &in_single_quotes;
	state.in_double_quotes = &in_double_quotes;
	process_expansion(&state);
	free(*str);
	*str = result;
	return (0);
}
