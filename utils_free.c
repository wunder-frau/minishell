#include "minishell.h"

void	free_arr_2d(void *ptr)
{
	void	**arr;
	size_t	i;

	arr = (void **)ptr;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	free_minishell(t_minishell *ms)
{
	if (!ms)
		return ;
	if (ms->env)
		free_arr_2d(ms->env);
	if (ms->root)
		free_ast(&(ms->root));
	free(ms);
}

void	free_ast(t_node **root)
{
	if (*root == NULL)
		return ;
	if ((*root)->type == T_REDIR)
	{
		if (((t_redir *)(*root))->redirs)
			free_arr_2d(((t_redir *)(*root))->redirs);
	}
	else if ((*root)->type == T_COMMAND)
	{
		free_ast(&((t_command *)(*root))->redir);
	}
	else
	{
		if ((*root)->left)
			free_ast(&((*root)->left));
		if ((*root)->right)
			free_ast(&((*root)->right));
	}
	free(*root);
	*root = NULL;
}
