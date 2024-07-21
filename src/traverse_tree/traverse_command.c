/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traverse_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 20:43:34 by istasheu          #+#    #+#             */
/*   Updated: 2024/07/22 00:35:42 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	traverse_command(char *cmd, char **redir, t_minishell *shell)
{
	int		status;
	char	**command;

	status = parse_cmd(cmd, &command, shell);
	shell->command = command;
	if (status == 0)
	{
		if (is_builtin(command[0]))
			status = run_builtin(redir, shell);
		else
			status = run_external_with_redir(command, redir, shell);
	}
	shell->exit_status = status;
	free_arr_2d(command);
	return (status);
}

int	run_external_with_redir(char **command, char **redir, t_minishell *shell)
{
	int		status;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (FORK_FAILURE);
	if (pid == 0)
	{
		status = 0;
		if (redir != NULL)
			status = apply_redirects(redir, shell);
		if (status != 0)
			exit(status);
		execution(shell, &command[0], &shell->cmd_data);
		print_err_shellg(command[0], ": execve() error occured\n");
		exit(EXECVE_FAILURE);
	}
	status = wait_children_and_fetch_exit_status(&pid, 1);
	return (status);
}

int	traverse_tree(t_node **root, t_minishell *shell)
{
	int		type;
	int		status;

	status = true;
	type = (*root)->type;
	if (type == T_PIPE)
		status = traverse_pipe(root, shell);
	else if (type == T_COMMAND)
		status = traverse_command(((t_command *)*root)->cmd,
				((t_redir *)((t_command *)*root)->redir)->redirs, shell);
	free_ast(root);
	return (status);
}
