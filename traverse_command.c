#include "minishell.h"

int	traverse_command(char *cmd, char **redir, t_minishell *ms)
{
	int		cmd_type;
	int		status;
	char **command;
	status = parse_cmd(cmd, &command, ms);
	if (status == 0)
	{
		cmd_type = is_builtin(command[0]);
		if (cmd_type > 0)
			status = run_builtin(command, redir, ms, cmd_type);
		else
			status = run_cmd_with_redir(command, redir, ms);
	}
	ms->exit_status = status;
	free_arr_2d(command);
	return (status);
}

int	run_cmd_with_redir(char **command, char **redir, t_minishell *ms)
{
	int		status;
	status = 0;
	if (redir != NULL)
		status = apply_redirects(redir, ms);
	if (status != 0)
		exit(status);
	printf("🤪🎉 Executing command___________________________>>>>>>:  %s 🤪🎉\n", command[0]);
	// if (execvp(command[0], command) == -1)
	// {
	// 	perror("execvp");
	// 	exit(EXECVE_FAILURE);
	// }
	execve(command[0], command, ms->env);
	print_err_msg(command[0], ": execve() error occured\n");
	exit(EXECVE_FAILURE);
	return (status);
}

int	traverse_tree(t_node **root, t_minishell *ms)
{
	int		type;
	int		status;

	status = true;
	type = (*root)->type;
	if (type == T_COMMAND)
		status = traverse_command(((t_command *)*root)->cmd,
				((t_redir *)((t_command *)*root)->redir)->redirs, ms);
	free_ast(root);
	return (status);
}