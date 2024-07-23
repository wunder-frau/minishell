/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_parse_node_data.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 01:56:56 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/23 02:03:42 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	check_redirection(char *str)
{
	int	i;
	int	inside_quotes;

	i = 0;
	inside_quotes = 0;
	while (str[i] != NULL_TERM)
	{
		if (!is_inside_quotes(str[i], &inside_quotes)
			&& (str[i] == REDIR_L || str[i] == REDIR_R))
			return (true);
		i++;
	}
	return (false);
}

int	set_node_data_pipe(t_parsed_data **data, char *str, int point, int type)
{
	t_parsed_data	*node_data;

	if (type == T_PIPE)
		str[point] = NULL_TERM;
	node_data = (t_parsed_data *)ft_calloc(1, sizeof(t_parsed_data));
	if (!node_data)
		return (-1);
	node_data->block_left = str;
	node_data->block_right = str + point + 1;
	node_data->type = type;
	*data = node_data;
	return (1);
}

int	set_node_cmd_redirects(t_parsed_data **data, char *str, int type)
{
	char					*redir;
	t_parsed_data			*node_data;
	int						status;

	node_data = ft_calloc(1, sizeof(t_parsed_data));
	if (!node_data)
		return (-1);
	status = convert_input_to_redirects(str, &redir, 0, 0);
	if (status == -1)
	{
		free(node_data);
		return (-1);
	}
	node_data->block_left = redir;
	node_data->block_right = str;
	node_data->type = type;
	*data = node_data;
	return (1);
}

int	set_node_cmd(t_parsed_data **data, char *str, int type)
{
	t_parsed_data	*node_data;

	node_data = ft_calloc(1, sizeof(t_parsed_data));
	if (!node_data)
		return (-1);
	node_data->block_left = NULL;
	node_data->block_right = str;
	node_data->type = type;
	*data = node_data;
	return (1);
}
