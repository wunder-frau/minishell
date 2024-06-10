#include "parser.h"
#include "minishell.h"
#include "lexer.h"

int	pipe_block(t_node_info **node, char *str, int type, int i)
{
	if (str[i] == PIPE)
	{
		if (round_brackets_check(str, i)
			&& quote_check(str, i, S_QUOTE)
			&& quote_check(str, i, D_QUOTE))
			return (set_node_info_and_or_pipe(node, str, i, type));
		else
			return (lexer(node, str, type, i - 1));
	}
	return (0);
}

bool	get_type(char *str, t_node_info **info)
{
	int	status;

	status = lexer(info, str, T_PIPE, ft_strlen(str) - 1);
	return (status);
}

int	create_tree(char *str, t_node **root, int *hd_num, t_minishell *ms)
{
	bool		status;
	int			type;
	t_node_info	*info;

    // status = get_type(str, &info);
	// if (status == false)
	// 	return (MALLOC_ERR);
	if (create_node(info, root) == false)
		return (200);
	type = info->type;

	if (status != 0)
	{
		free_tree(root);
		return (status);
	}
	return (0);
}