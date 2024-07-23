/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 22:46:08 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/21 22:46:09 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_pipe	*init_node_pipe(void)
{
	t_pipe	*node;

	node = (t_pipe *)ft_calloc(1, sizeof(t_pipe));
	if (!node)
		return (NULL);
	node->type = T_PIPE;
	return (node);
}

t_command	*init_node_cmd(void)
{
	t_command	*node;

	node = (t_command *)ft_calloc(1, sizeof(t_command));
	if (!node)
		return (NULL);
	node->type = T_COMMAND;
	return (node);
}

t_redir	*init_node_redir(void)
{
	t_redir	*node;

	node = (t_redir *)ft_calloc(1, sizeof(t_redir));
	if (!node)
		return (0);
	node->type = T_REDIR;
	return (node);
}

bool	init_node(t_parsed_data *data, t_node **root)
{
	t_node	*node;

	node = NULL;
	if (data->type == T_PIPE)
		node = (t_node *)init_node_pipe();
	else if (data->type == T_COMMAND)
		node = (t_node *)init_node_cmd();
	if (node == NULL)
	{
		free(data);
		return (false);
	}
	*root = node;
	return (true);
}
