/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 01:56:45 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/23 01:59:57 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_symbol_pairing(char *str, int point, int symbol)
{
	int	last_str_index;
	int	pair_0;
	int	pair_1;

	last_str_index = (int)ft_strlen(str) - 1;
	pair_0 = 0;
	pair_1 = 0;
	while (last_str_index >= point)
	{
		if (str[last_str_index] == symbol)
			pair_0++;
		last_str_index--;
	}
	while (--point > 0)
	{
		if (str[point] == symbol)
			pair_1++;
	}
	return (ft_is_odd(pair_0) == false && ft_is_odd(pair_1) == false);
}

int	block_pipe(t_parsed_data **node, char *str, int type, int i)
{
	if (str[i] == PI)
	{
		if (check_symbol_pairing(str, i, S_QUO)
			&& check_symbol_pairing(str, i, D_QUO))
			return (set_node_data_pipe(node, str, i, type));
		else
			return (lexer(node, str, type, i - 1));
	}
	return (0);
}

int	block_cmd(t_parsed_data **node, char *str, int type)
{
	if (check_redirection(str))
		return (set_node_cmd_redirects(node, str, type));
	else
		return (set_node_cmd(node, str, T_COMMAND));
	return (0);
}

int	lexer(t_parsed_data **node, char *str, int type, int i)
{
	int	status;
	int	last_str_index;

	last_str_index = ft_strlen(str) - 1;
	while (i >= 0)
	{
		if (type == T_PIPE)
			status = block_pipe(node, str, type, i);
		else if (type == T_COMMAND)
			status = block_cmd(node, str, type);
		if (status > 0)
			return (status);
		else if (status < 0)
			return (-1);
		i--;
	}
	return (lexer(node, str, type + 1,
			last_str_index));
}
