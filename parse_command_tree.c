#include "minishell.h"

bool	get_type(char *str, t_node_info **info)
{
	int	status;

	status = lexer(info, str, T_PIPE, ft_strlen(str) - 1);
	return (status);
}


void	ft_free_2d_array(void *ptr)
{
	void	**arr;
	size_t	i;

	arr = (void **)ptr;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
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
	printf("\n");
	status = create_tree(info->str_left, &((*root)->left), hd_num, ms);
	if (status == 0)
	{
		printf("right_____");
		printf("\n");
		status = create_tree(info->str_right, &((*root)->right), hd_num, ms);
	}
	return (status);
}

int add_command(t_node_info *info, t_node **root, int *hd_num, t_minishell *ms)
{
		int status;
		t_redir *redir_node;
		char **redirs;
		(void)*hd_num;
		(void)*ms;

		status = prepare_redirects(info->str_left, hd_num, &redirs, ms);
		if (status != 0)
		    return (status);

		redir_node = init_t_redir();
		if (redir_node == NULL)
		{
		    if (redirs)
		        ft_free_2d_array(redirs);
		    return (200);
		}

		redir_node->redirs = redirs;
		((t_command *)(*root))->redir = (t_node *)redir_node;
		((t_command *)(*root))->cmd = info->str_right;
		return (0);
}

// int	add_command(t_node_info *info, t_node **root, int *hd_num, t_minishell *ms)
// {
// 	int		status;
// 	t_redir	*redir_node;
// 	char	**redirs;

// 	status = prepare_redirects(info->str_left, hd_num, &redirs, ms);
// 	if (status != 0)
// 		return (status);
// 	redir_node = init_t_redir();
// 	if (redir_node == NULL)
// 	{
// 		if (redirs)
// 			ft_free_2d_array(redirs);
// 		return (200);
// 	}
// 	redir_node->redirs = redirs;
// 	((t_command *)(*root))->redir = (t_node *)redir_node;
// 	((t_command *)(*root))->cmd = info->str_right;
// 	return (0);
// }

void print_node(t_node *node)
{
    if (!node)
    {
        printf("NULL\n");
        return;
    }

    printf("Node type: %d\n", node->type);

    printf("Left child:\n");
    print_node(node->left);
    printf("Right child:\n");
    print_node(node->right);
}

t_pipe  *create_pipe_node(void)
{
	t_pipe	*node;

	printf("Entering create_pipe_node\n");
	node = (t_pipe *)ft_calloc(1, sizeof(t_pipe));
	if (!node)
		return (NULL);
	node->type = T_PIPE;
	printf("Pipe node created with type=%d\n", node->type);
	printf("\n");
	return (node);
}

t_command	*init_t_command(void)
{
	t_command	*node;

	node = (t_command *)ft_calloc(1, sizeof(t_command));
	if (!node)
		return (NULL);
	node->type = T_COMMAND;
	return (node);
}

t_redir	*init_t_redir(void)
{
	t_redir	*node;

	node = (t_redir *)ft_calloc(1, sizeof(t_redir));
	if (!node)
		return (0);
	node->type = T_REDIR;
	return (node);
}

bool	create_node(t_node_info *info, t_node **root)
{
	t_node	*node;

	printf("Entering create_node with info->type=%d\n", info->type);
	node = NULL;

	if (info->type == T_PIPE)
	{
		node = (t_node *)create_pipe_node();

	}
	else if (info->type == T_COMMAND)
	{
		node = (t_node *)init_t_command();
			print_node(node);
	printf("\n");
	}
	if (node == NULL)
	{
		printf("Node creation failed, freeing info\n");
		free(info);
		return (false);
	}
	*root = node;

	printf("Node created successfully, root set\n");
	printf("\n");
	print_node(node);
	printf("\n");
	return (true);
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
	//(void)**root;
    status = get_type(str, &info);
	if (status == false)
		return (200);
	if (create_node(info, root) == false)
		return (200);
	type = info->type;
	if (type == T_PIPE)
		status = pipe_tree(info, root, hd_num, ms);
	else if (type == T_COMMAND)
		status = add_command(info, root, hd_num, ms);
	free(info);
	if (status != 0)
	{
		free_tree(root);
		return (status);
	}
	return (0);
}

