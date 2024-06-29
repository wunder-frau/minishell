#include "minishell.h"

t_pipe  *init_node_pipe(void)
{
	t_pipe	*node;

	printf("Entering init_node_pipe\n");
	node = (t_pipe *)ft_calloc(1, sizeof(t_pipe));
	if (!node)
		return (NULL);
	node->type = T_PIPE;
	printf("Pipe node created with type=%d\n", node->type);
	printf("\n");
	return (node);
}

t_command	*init_node_cmd(void)
{
	t_command	*node;

	node = (t_command *)ft_calloc(1, sizeof(t_command));
	if (!node)
		return (NULL);
	node->type = T_COMMAND;
	printf("Cmd node created with type=%d\n", node->type);
	printf("\n");
	return (node);
}

t_redir	*init_node_redir(void)
{
	t_redir	*node;

	node = (t_redir *)ft_calloc(1, sizeof(t_redir));
	if (!node)
		return (0);
	node->type = T_REDIR;
	printf("Redir node created with type=%d\n", node->type);
	printf("\n");
	return (node);
}

bool	init_node(t_parsed_data *info, t_node **root)
{
	t_node	*node;

	printf("Entering init_node with info->type=%d\n", info->type);
	node = NULL;

	if (info->type == T_PIPE)
	{
		node = (t_node *)init_node_pipe();

	}
	else if (info->type == T_COMMAND)
	{
		node = (t_node *)init_node_cmd();
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