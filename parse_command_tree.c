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
void print_node(t_node *node)
{
    if (!node)
    {
        printf("NULL\n");
        return;
    }

    printf("Node type: %d\n", node->type);
    printf("Left child: ");
    print_node(node->left);
    printf("Right child: ");
    print_node(node->right);
}

void print_root(t_node *root)
{
    printf("Printing root node:\n");
    print_node(root);
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

bool	create_node(t_node_info *info, t_node **root)
{
	t_node	*node;

	printf("Entering create_node with info->type=%d\n", info->type);
	node = NULL;

	if (info->type == T_PIPE)
		node = (t_node *)create_pipe_node();
	if (node == NULL)
	{
		printf("Node creation failed, freeing info\n");
		free(info);
		return (false);
	}
	*root = node;
	printf("Node created successfully, root set\n");
    printf("Root node address: %p\n", (void *)*root);
    if (*root)
    {
        printf("Root node type: %d\n", ((t_pipe *)*root)->type);
        // Add additional fields to print if necessary
    }

	printf("Node created successfully, root set\n");
	printf("\n");
	print_root(*root);
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
	free(info);
	if (status != 0)
	{
		free_tree(root);
		return (status);
	}
	return (0);
}

