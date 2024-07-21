/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 22:46:13 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/22 00:42:27 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	parse_ast(char *str, t_parsed_data **data)
{
	int	status;
	int	last_str_index;

	last_str_index = ft_strlen(str) - 1;
	status = lexer(data, str, T_PIPE, last_str_index);
	return (status);
}

int	build_ast(char *str, t_node **root, int *hd_count, t_minishell *shell)
{
	bool				status;
	t_parsed_data		*data;
	int					type;

	status = parse_ast(str, &data);
	if (status == false)
		return (200);
	if (init_node(data, root) == false)
		return (200);
	type = data->type;
	if (type == T_PIPE)
		status = assemble_ast_pipe(data, root, hd_count, shell);
	else if (type == T_COMMAND)
		status = assemble_ast_command(data, root, hd_count, shell);
	free(data);
	if (status != SUCCESS)
	{
		free_ast(root);
		return (status);
	}
	return (SUCCESS);
}
