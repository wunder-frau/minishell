/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_expansion_3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 19:37:05 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/07/24 19:38:34 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	process_single_quote(t_expansion_state *state, char c)
{
	*(state->in_single_quotes) = !*(state->in_single_quotes);
	append_char_to_result(state->result, c);
}

void	process_double_quote(t_expansion_state *state, char c)
{
	*(state->in_double_quotes) = !*(state->in_double_quotes);
	append_char_to_result(state->result, c);
}

void	process_dollar(t_expansion_state *state)
{
	int		var_len;
	char	*temp;
	char	*old_result;

	temp = expand_variable(*(state->str) + *(state->i), state->ms, \
	state->last_status, &var_len);
	if (!temp)
		return ;
	old_result = *(state->result);
	*(state->result) = ft_strjoin(*(state->result), temp);
	free(old_result);
	free(temp);
	*(state->i) += var_len;
}

void	process_backslash_dollar(t_expansion_state *state)
{
	append_char_to_result(state->result, '$');
	*(state->i) += 2;
}

void	process_other_chars(t_expansion_state *state)
{
	append_char_to_result(state->result, (*(state->str))[*(state->i)]);
	(*(state->i))++;
}
