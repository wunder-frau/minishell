/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 00:11:35 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/27 14:14:29 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	free_minishell(t_minishell *shell)
{
	if (!shell)
		return ;
	if (shell->hashmap)
	{
		free_hmap(*(shell->hashmap));
		free(shell->hashmap);
	}
	if (shell->pwd)
		free(shell->pwd);
	if (shell->oldpwd)
		free(shell->oldpwd);
	if (shell->root)
		free_ast(&(shell->root));
	free(shell);
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

void	free_hmap(t_hmap *node)
{
	while (node)
	{
		if (node->key)
			free(node->key);
		if (node->value)
			free(node->value);
		free(node);
		node = node->next;
	}
}
