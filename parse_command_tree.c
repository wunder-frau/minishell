#include "minishell.h"

bool	get_type(char *str, t_node_info **info)
{
	int	status;

	status = lexer(info, str, T_PIPE, ft_strlen(str) - 1);
	return (status);
}

void free_tree(t_node **root) {
    if (*root == NULL)
        return;

		if ((*root)->left)
			free_tree(&((*root)->left));
		if ((*root)->right)
			free_tree(&((*root)->right));

    free(*root);
    *root = NULL;
}

int	pipe_tree(t_node_info *info, t_node **root, int *hd_num,
		t_minishell *ms)
{
	int	status;
	(void)*hd_num;
	(void)*ms;

	printf("left_____");
	status = create_tree(info->str_left, &((*root)->left), hd_num, ms);
	if (status == 0)
	{
		printf("right_____");
		status = create_tree(info->str_right, &((*root)->right), hd_num, ms);
	}
	return (status);
}

// t_pipe  *create_pipe_node(void)
// {
// 	t_pipe	*node;

// 	node = (t_pipe *)ft_calloc(1, sizeof(t_pipe));
// 	if (!node)
// 		return (NULL);
// 	node->type = T_PIPE;
// 	return (node);
// }

// bool	create_node(t_node_info *info, t_node **root)
// {
// 	t_node	*node;

// 	node = NULL;

// 	if (info->type == T_PIPE)
// 		node = (t_node *)create_pipe_node();
// 	if (node == NULL)
// 	{
// 		free(info);
// 		return (false);
// 	}
// 	*root = node;
// 	return (true);
// }

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
	//(void)**root;
    status = get_type(str, &info);
	if (status == false)
		return (200);
	// if (create_node(info, root) == false)
	// 	return (200);
	type = info->type;
	if (type == T_PIPE)
		status = pipe_tree(info, root, hd_num, ms);
	free(info);
	if (status != 0)
	{
		free_tree(root);
		return (status);
	}
	return (0);
}

