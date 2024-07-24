/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traverse_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 20:41:25 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/24 03:15:42 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	traverse_pipe(t_node **root, t_minishell *shell)
{
	int		status;
	int		pipefd[2];
	pid_t	pids[2];
	t_node	*node;

	if (shell->is_parent == false)
		signal_interceptor(0);
	node = *root;
	if (pipe(pipefd) == -1)
		return (PIPE_FAILURE);
	status = traverse_lhs(&(node->left), shell, pipefd, pids);
	if (status == 0)
		status = traverse_rhs(&(node->right), shell, pipefd, pids);
	status = wait_children_and_fetch_exit_status(pids, 2);
	return (status);
}

int	traverse_lhs(t_node **node, t_minishell *shell, int pipefd[2], int pids[2])
{
	int	status;

	pids[0] = fork();
	if (pids[0] == -1)
		return (FORK_FAILURE);
	if (pids[0] == 0)
	{
		signal_interceptor(0);
		shell->is_parent = false;
		close(pipefd[0]);
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			close(pipefd[1]);
			exit(DUP_FAILURE);
		}
		close(pipefd[1]);
		status = traverse_tree(node, shell);
		exit(status);
	}
	else
		close(pipefd[1]);
	return (0);
}

int	traverse_rhs(t_node **node, t_minishell *shell, int pipefd[2], int pids[2])
{
	int	status;

	pids[1] = fork();
	if (pids[1] == -1)
		return (FORK_FAILURE);
	if (pids[1] == 0)
	{
				signal_interceptor(0);
		shell->is_parent = false;
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			close(pipefd[0]);
			exit(DUP_FAILURE);
		}
		close(pipefd[0]);
		status = traverse_tree(node, shell);
		exit(status);
	}
	else
		close(pipefd[0]);
	return (0);
}

int	wait_children_and_fetch_exit_status(pid_t *pids, int num)
{
	int	i;
	int	status;

	i = 0;
	while (i < num)
	{
		if (pids[i] == -1)
			return (FORK_FAILURE);
		waitpid(pids[i], &status, 0);
		i++;
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_FAILURE);
}
