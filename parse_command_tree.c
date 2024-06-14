#include "minishell.h"

bool	get_type(char *str, t_node_info **info)
{
	int	status;

	status = lexer(info, str, T_PIPE, ft_strlen(str) - 1);
	return (status);
}
/**
* @param `status` is initially used to store the return value of the get_type
*		 function, which determines the type of the current node.
*/
int	create_tree(char *str, t_node **root, int *hd_num, t_minishell *ms)
{
	bool		status;
	int			type;
	t_node_info	*info;
	(void)*hd_num;
	(void)*ms;
	(void)**root;
    status = get_type(str, &info);
	if (status == false)
		return (200);
	type = info->type;

	if (status != 0)
	{
		//free_tree(root);
		return (status);
	}
	return (0);
}