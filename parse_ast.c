#include "minishell.h"

bool	parse_ast(char *str, t_parsed_data **data)
{
	int	status;

	status = lexer(data, str, T_PIPE, ft_strlen(str) - 1);
	return (status);
}

int	build_ast(char *str, t_node **root, int *hd_num, t_minishell *ms)
{
	bool				status;
	int					type;
	t_parsed_data	*data;
	(void)*hd_num;
	(void)*ms;

	status = parse_ast(str, &data);
	if (status == false)
		return (200);
	if (init_node(data, root) == false)
		return (200);
	type = data->type;
	if (type == T_PIPE)
		status = assemble_ast_pipe(data, root, hd_num, ms);
	else if (type == T_COMMAND)
		status = assemble_ast_command(data, root, hd_num, ms);
	free(data);
	if (status != 0)
	{
		free_ast(root);
		return (status);
	}
	return (0);
}
