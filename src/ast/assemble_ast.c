/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assemble_ast.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 22:46:01 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/21 22:46:02 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	assemble_ast_pipe(t_parsed_data *data, t_node **root, int *hd_num,
		t_minishell *ms)
{
	int	status;

	status = build_ast(data->block_left, &((*root)->left), hd_num, ms);
	if (status == 0)
		status = build_ast(data->block_right, &((*root)->right), hd_num, ms);
	return (status);
}

int	assemble_ast_command(t_parsed_data *data, t_node **root, int *hd_num,
		t_minishell *ms)
{
	int				status;
	t_redir			*redir_node;
	char			**redirs;

	status = prepare_redirects(data->block_left, hd_num, &redirs, ms);
	if (status != 0)
		return (status);
	redir_node = init_node_redir();
	if (redir_node == NULL)
	{
		if (redirs)
			free_arr_2d(redirs);
		return (200);
	}
	redir_node->redirs = redirs;
	((t_command *)(*root))->redir = (t_node *)redir_node;
	((t_command *)(*root))->cmd = data->block_right;
	return (0);
}
