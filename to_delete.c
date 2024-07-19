#include "minishell.h"

void print_node(t_node *node)
{
	if (!node)
	{
			//printf("NULL\n");
			return;
	}

	//printf("Node type: %d\n", node->type);

	//printf("Left child:\n");
	print_node(node->left);
	//printf("Right child:\n");
	print_node(node->right);
}